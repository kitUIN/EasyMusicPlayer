#pragma once

#include<vector>

template<class Type>
class Splay
{
public:
    ~Splay(void)
    {
        Clear(root);
        delete null;
    }
    int size(void)
    {
        return count;
    }
    void clear(void)
    {
        Clear(root);
        root = null;
    }
    bool empty(void)
    {
        return count == 0;
    }
    Type pre(Type val)
    {
        root = splay(FindPre(root, val));
        return root->val;
    }
    Type nxt(Type val)
    {
        root = splay(FindNxt(root, val));
        return root->val;
    }
    void erase(Type val)
    {
        count--; root = Delete(FindByValu(root, val));
    }
    void insert(Type val)
    {
        count++; root = splay(Insert(root, val));
    }
    int getrankbyval(Type val)
    {
        root = splay(FindByValu(root, val));
        return root->lch->siz + 1;
    }
    Type& getvalbyrank(int rank)
    {
        root = splay(FindByRank(root, rank));
        return root->val;
    }
    Type lower_bound(Type val)
    {
        root = splay(FindLower(root, val));
        return root->val;
    }
    Type upper_bound(Type val)
    {
        root = splay(FindUpper(root, val));
        return root->val;
    }
    void getall(std::vector<Type>& lib)
    {
        DFS(root, lib);
    }
    bool find(Type val)
    {
        return FindByValu(root, val) != null;
    }
private:
    struct Node
    {
        int siz = 0;
        Type val = Type();
        Node* fa = NULL;
        Node* lch = NULL;
        Node* rch = NULL;
    };
    /*====================*/
    typedef bool CHILD;
    const CHILD LCH = true;
    const CHILD RCH = false;
    /*====================*/
    int count = 0;
    Node* null = new Node;
    Node* root = null;
    /*====================*/
    CHILD Child(Node* cur)
    {
        Node* pre = cur->fa;
        if (pre->lch == cur)
        {
            return LCH;
        }
        else
        {
            return RCH;
        }
    }

    void PushUp(Node* cur)
    {
        cur->siz = cur->lch->siz + cur->rch->siz + 1;
    }

    void Del(Node* cur, Node* pre, CHILD WCH)
    {
        cur->fa = null;
        if (WCH == LCH)pre->lch = null;
        if (WCH == RCH)pre->rch = null;
    }
    void Add(Node* cur, Node* pre, CHILD WCH)
    {
        cur->fa = pre;
        if (WCH == LCH)pre->lch = cur;
        if (WCH == RCH)pre->rch = cur;
    }

    void LRotate(Node* cur)
    {
        Node* pre = cur->fa, * nxt = cur->lch, * anc = pre->fa;
        CHILD WCH = Child(pre);
        Del(nxt, cur, LCH); Del(cur, pre, RCH); Del(pre, anc, WCH);
        Add(nxt, pre, RCH); Add(pre, cur, LCH); Add(cur, anc, WCH);
        PushUp(pre); PushUp(cur);
    }
    void RRotate(Node* cur)
    {
        Node* pre = cur->fa, * nxt = cur->rch, * anc = pre->fa;
        CHILD WCH = Child(pre);
        Del(nxt, cur, RCH); Del(cur, pre, LCH); Del(pre, anc, WCH);
        Add(nxt, pre, LCH); Add(pre, cur, RCH); Add(cur, anc, WCH);
        PushUp(pre); PushUp(cur);
    }

    void Rotate(Node* cur)
    {
        if (Child(cur) == LCH)
        {
            RRotate(cur);
        }
        else
        {
            LRotate(cur);
        }
    }

    void Clear(Node* cur)
    {
        if (cur != null)
        {
            Clear(cur->lch);
            Clear(cur->rch);
            delete cur;
        }
    }

    Node* Creat(Type val)
    {
        Node* cur = new Node;
        cur->lch = null;
        cur->rch = null;
        cur->fa = null;
        cur->val = val;
        cur->siz = 1;
        return cur;
    }

    Node* splay(Node* cur)
    {
        while (true)
        {
            Node* pre = cur->fa;
            if (cur->fa == null)break;
            if (pre->fa == null)break;
            CHILD CHpre = Child(pre);
            CHILD CHcur = Child(cur);
            if (CHpre == CHcur)
            {
                Rotate(pre); Rotate(cur); continue;
            }
            if (CHpre != CHcur)
            {
                Rotate(cur); Rotate(cur); continue;
            }
        }
        if (cur->fa != null)Rotate(cur); return cur;
    }

    Node* Insert(Node* cur, Type val)
    {
        CHILD WCH = LCH; Node* pre = null;
        while (cur != null)
        {
            if (val < cur->val)
            {
                pre = cur; cur = cur->lch; WCH = LCH;
            }
            else
            {
                pre = cur; cur = cur->rch; WCH = RCH;
            }
        }
        cur = Creat(val); Add(cur, pre, WCH); return cur;
    }

    Node* Delete(Node* cur)
    {
        splay(cur);
        Node* lch = cur->lch;
        Node* rch = cur->rch;
        delete cur; return Merge(lch, rch);
    }

    Node* Merge(Node* ltree, Node* rtree)
    {
        if (ltree == null)
        {
            rtree->fa = null; return rtree;
        }
        if (rtree == null)
        {
            ltree->fa = null; return ltree;
        }
        ltree->fa = null; rtree->fa = null;
        if (ltree->siz < rtree->siz)
        {
            Node* cur = FindMax(ltree); splay(cur);
            Add(rtree, cur, RCH); PushUp(cur); return cur;
        }
        else
        {
            Node* cur = FindMin(rtree); splay(cur);
            Add(ltree, cur, LCH); PushUp(cur); return cur;
        }
    }

    Node* FindByValu(Node* cur, Type val)
    {
        Node* res = null;
        while (cur != null)
        {
            if (val == cur->val)
            {
                res = cur, cur = cur->lch;
            }
            else
            {
                if (val < cur->val)
                {
                    cur = cur->lch;
                }
                else
                {
                    cur = cur->rch;
                }
            }
        }
        return res;
    }
    Node* FindByRank(Node* cur, int rank)
    {
        while (cur != null)
        {
            if (cur->lch->siz + 1 == rank)
            {
                return cur;
            }
            else
            {
                if (cur->lch->siz < rank)
                {
                    rank -= cur->lch->siz + 1;
                    cur = cur->rch;
                }
                else
                {
                    cur = cur->lch;
                }
            }
        }
        return null;
    }

    Node* FindLower(Node* cur, Type val)
    {
        Node* res = null;
        while (cur != null)
        {
            if (cur->val < val)
            {
                cur = cur->rch;
            }
            else
            {
                res = cur;
                cur = cur->lch;
            }
        }
        return res;
    }
    Node* FindUpper(Node* cur, Type val)
    {
        Node* res = null;
        while (cur != null)
        {
            if (val < cur->val)
            {
                res = cur;
                cur = cur->lch;
            }
            else
            {
                cur = cur->rch;
            }
        }
        return res;
    }

    Node* FindMin(Node* cur)
    {
        while (cur->lch != null)
        {
            cur = cur->lch;
        }
        return cur;
    }
    Node* FindMax(Node* cur)
    {
        while (cur->rch != null)
        {
            cur = cur->rch;
        }
        return cur;
    }

    Node* FindPre(Node* cur, Type val)
    {
        Node* res = null;
        while (cur != null)
        {
            if (cur->val < val)
            {
                res = cur;
                cur = cur->rch;
            }
            else
            {
                cur = cur->lch;
            }
        }
        return res;
    }
    Node* FindNxt(Node* cur, Type val)
    {
        Node* res = null;
        while (cur != null)
        {
            if (val < cur->val)
            {
                res = cur;
                cur = cur->lch;
            }
            else
            {
                cur = cur->rch;
            }
        }
        return res;
    }

    void DFS(Node* cur, std::vector<Type>& lib)
    {
        if (cur != null)
        {
            DFS(cur->lch, lib);
            lib.push_back(cur->val);
            DFS(cur->rch, lib);
        }
    }
};
