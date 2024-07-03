#ifndef CS10123F_PA2_BST
#define CS10123F_PA2_BST

#include <vector>
#include <stack>
#include <iostream>

const double alpha = 0.75;

struct Node
{
public:
    long long value, size, sum_count, sum, count;
    Node *left;
    Node *right;

    Node(long long value) : value(value), size(1), sum_count(1), sum(value), count(1), left(nullptr), right(nullptr) {}

    std::vector<Node *> inorder_traversal()
    {
        std::vector<Node *> res;
        std::stack<Node *> stack;
        Node *curr = this;

        while (curr != nullptr || !stack.empty())
        {
            while (curr != nullptr)
            {
                stack.push(curr);
                curr = curr->left;
            }

            curr = stack.top();
            stack.pop();
            res.push_back(curr);
            curr = curr->right;
        }

        return res;
    }

    bool isBalanced()
    {
        if (left == nullptr && right == nullptr)
        {
            return true;
        }
        else if (left == nullptr)
        {
            return right->size <= alpha * size;
        }
        else if (right == nullptr)
        {
            return left->size <= alpha * size;
        }
        else
        {
            return left->size <= alpha * size && right->size <= alpha * size;
        }
    }

    void update()
    {
        size = (left ? left->size : 0) + 1 + (right ? right->size : 0);
        sum_count = (left ? left->sum_count : 0) + count + (right ? right->sum_count : 0);
        sum = (left ? left->sum : 0) + value * count + (right ? right->sum : 0);
    }
};

class ScapegoatTree
{
private:
    Node *root;
    long long search(Node *root, long long k) const
    {
        long long left_sum_count = root->left ? root->left->sum_count : 0;
        if (k > left_sum_count && k <= left_sum_count + root->count)
        {
            return root->value;
        }
        else if (k <= left_sum_count)
        {
            return search(root->left, k);
        }
        else
        {
            return search(root->right, k - left_sum_count - root->count);
        }
    }

    Node *rebuild(size_t l, size_t r, std::vector<Node *> &s)
    {
        if (l > r)
        {
            return nullptr;
        }

        size_t mid = (l + r) / 2;
        Node *res = s[mid];
        res->left = l == mid ? nullptr : rebuild(l, mid - 1, s);
        res->right = mid == r ? nullptr : rebuild(mid + 1, r, s);
        res->update();

        return res;
    }

    Node *find_scapegoat(Node *node) const
    {
        Node *res = root;

        while (res != nullptr)
        {
            if (!res->isBalanced())
            {
                return res;
            }
            else
            {
                res = res->value > node->value ? res->left : res->right;
            }
        }

        return res;
    }

    Node *null_insert(long long value)
    {
        if (root == nullptr)
        {
            root = new Node(value);
            return root;
        }
        Node *res = root;
        while (true)
        {
            ++res->size;
            ++res->sum_count;
            res->sum += value;
            if (res->value > value)
            {
                if (res->left == nullptr)
                {
                    res->left = new Node(value);
                    return res->left;
                }
                else
                {
                    res = res->left;
                }
            }
            else if (res->value < value)
            {
                if (res->right == nullptr)
                {
                    res->right = new Node(value);
                    return res->right;
                }
                else
                {
                    res = res->right;
                }
            }
            else
            {
                ++res->count;
                return res;
            }
        }
    }

    long long sum_less_than(long long k) const
    {
        long long res = 0;
        Node *curr = root;
        while (true)
        {
            if (curr->value < k)
            {
                if (curr->right != nullptr)
                {
                    res += curr->sum - curr->right->sum;
                    curr = curr->right;
                }
                else
                {
                    res += curr->sum;
                    return res;
                }
            }
            else if (curr->value > k)
            {
                if (curr->left != nullptr)
                {
                    curr = curr->left;
                }
                else
                {
                    return res;
                }
            }
            else
            {
                res += curr->left ? curr->left->sum : 0;
                return res;
            }
        }
    }

public:
    ScapegoatTree() : root(nullptr) {}

    bool empty() const
    {
        return !root;
    }

    bool find(long long value) const
    {
        Node *res = root;

        while (res != nullptr)
        {
            if (res->value == value)
            {
                return true;
            }
            else
            {
                res = res->value > value ? res->left : res->right;
            }
        }

        return false;
    }

    long long querykth(long long k) const
    {
        return search(root, k);
    }

    long long queryksum(long long k) const
    {
        Node *node = root;
        long long left_sum = root->left ? root->left->sum : 0;
        long long right_sum = root->right ? root->right->sum : 0;
        long long res = k * root->sum;

        while (node != nullptr)
        {
            if (k * (left_sum + node->value * node->count) - right_sum < 0)
            {
                left_sum += node->value * node->count;
                if (node->right != nullptr)
                {
                    node = node->right;
                }
                else
                {
                    break;
                }
                left_sum += node->left ? node->left->sum : 0;
                right_sum -= node->value * node->count + (node->left ? node->left->sum : 0);
            }
            else if (k * left_sum - right_sum - node->value * node->count > 0)
            {
                res = std::min(res, k * left_sum - right_sum - node->value * node->count);
                right_sum += node->value * node->count;
                if (node->left != nullptr)
                {
                    node = node->left;
                }
                else
                {
                    break;
                }
                right_sum += node->right ? node->right->sum : 0;
                left_sum -= node->value * node->count + (node->right ? node->right->sum : 0);
            }
            else
            {
                for (int i = 0; i <= node->count; ++i)
                {
                    long long temp_left_sum = left_sum + node->value * i;
                    long long temp_right_sum = right_sum + node->value * (node->count - i);
                    if (k * temp_left_sum - temp_right_sum >= 0)
                    {
                        res = std::min(res, k * temp_left_sum - temp_right_sum);
                        break;
                    }
                }
                right_sum += node->value * node->count;
                if (node->left != nullptr)
                {
                    node = node->left;
                }
                else
                {
                    break;
                }
                right_sum += node->right ? node->right->sum : 0;
                left_sum -= node->value * node->count + (node->right ? node->right->sum : 0);
            }
        }

        return res;
    }

    // long long queryksum(long long k) const
    // {
    //     std::vector<Node *> node_list = root->inorder_traversal();
    //     long long res = -root->sum;
    //     for (Node *&node : node_list)
    //     {
    //         for (long long i = 0; i < node->count; ++i)
    //         {
    //             res += (k + 1) * node->value;
    //             if (res >= 0)
    //             {
    //                 return res;
    //             }
    //         }
    //     }
    //     return res;
    // }

    Node *parent(long long value) const
    {
        Node *res = root;
        Node *res_parent = nullptr;

        while (value != res->value && res != nullptr)
        {
            res_parent = res;
            res = res->value > value ? res->left : res->right;
        }

        return res_parent;
    }

    void insert(long long value)
    {
        auto node = null_insert(value);

        auto scapegoat = find_scapegoat(node);

        if (scapegoat != nullptr)
        {
            auto node_list = scapegoat->inorder_traversal();

            auto parent = this->parent(scapegoat->value);

            if (parent == nullptr)
            {
                root = rebuild(0, node_list.size() - 1, node_list);
            }
            else
            {
                if (parent->left == scapegoat)
                {
                    parent->left = rebuild(0, node_list.size() - 1, node_list);
                }
                else
                {
                    parent->right = rebuild(0, node_list.size() - 1, node_list);
                }
            }
        }
    }

    void erase(long long value)
    {
        std::stack<Node *> path;
        Node *node = root;

        while (node != nullptr && node->value != value)
        {
            path.push(node);
            node = value < node->value ? node->left : node->right;
        }

        if (node == nullptr)
        {
            return;
        }

        if (node->count > 1)
        {
            --node->count;
            node->update();
            while (!path.empty())
            {
                Node *current_node = path.top();
                path.pop();
                current_node->update();
            }
            return;
        }

        Node *parent_node = path.empty() ? nullptr : path.top();
        if (node->left == nullptr && node->right == nullptr)
        {
            if (parent_node == nullptr)
            {
                root = nullptr;
            }
            else if (parent_node->left == node)
            {
                parent_node->left = nullptr;
            }
            else
            {
                parent_node->right = nullptr;
            }

            delete node;
        }
        else if (node->left == nullptr || node->right == nullptr)
        {
            Node *child_node = node->left ? node->left : node->right;
            if (parent_node == nullptr)
            {
                root = child_node;
            }
            else if (parent_node->left == node)
            {
                parent_node->left = child_node;
            }
            else
            {
                parent_node->right = child_node;
            }

            delete node;
        }
        else
        {
            std::stack<Node *> min_path;
            Node *min_node = node->right;

            while (min_node->left != nullptr)
            {
                min_path.push(min_node);
                min_node = min_node->left;
            }

            node->value = min_node->value;
            node->count = min_node->count;

            Node *min_parent_node = min_path.empty() ? node : min_path.top();
            if (min_parent_node->left == min_node)
            {
                min_parent_node->left = min_node->right;
            }
            else
            {
                min_parent_node->right = min_node->right;
            }

            while (!min_path.empty())
            {
                Node *current_node = min_path.top();
                min_path.pop();
                current_node->update();
            }

            delete min_node;

            node->update();
        }

        while (!path.empty())
        {
            Node *current_node = path.top();
            path.pop();
            current_node->update();
        }
    }

    void clear()
    {
        std::stack<Node *> stack;
        stack.push(root);

        while (!stack.empty())
        {
            Node *node = stack.top();
            stack.pop();

            if (node == nullptr)
            {
                continue;
            }

            stack.push(node->left);
            stack.push(node->right);
            delete node;
        }

        root = nullptr;
    }

    ~ScapegoatTree()
    {
        clear();
    }
};

ScapegoatTree tree;

// you can do data structure initialization in this function
void init(std::size_t n [[maybe_unused]])
{
}

// You can deconstruct your data structure in this function
void clear()
{
    tree.clear();
}

// opt==1 insert x
// opt==2 erase x
// opt==3 query x
// opt==4 query xth
// opt==5 query x*sum(s1...st) - sum(s_t+1...s_m)
long long SetOperation(std::size_t opt, long long x)
{
    if (opt == 1)
    {
        tree.insert(x);
        return 0;
    }
    else if (opt == 2)
    {
        tree.erase(x);
        return 0;
    }
    else if (opt == 3)
    {
        return tree.find(x);
    }
    else if (opt == 4)
    {
        return tree.querykth(x);
    }
    else if (opt == 5)
    {
        return tree.queryksum(x);
    }
    return 0;
}

#endif // CS10123F_PA2_BST