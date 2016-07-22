//=============================================================
//  Binary Tree Classes     bintreek.h   v3.00
//      Template for keyed binary tree types
//      Copyright 1993-1995 Scott Robert Ladd
//=============================================================

#ifndef BINTREEKEY_H
#define BINTREEKEY_H

#ifdef __WATCOMC__
#pragma warning 594 9
#pragma warning 604 9
#endif

#include "stddef.h"
#include "limits.h"
#include "treex.h"

//-------------------------------------------------------------
//  TreeNodeKey
//      A node in a binary tree
//-------------------------------------------------------------

template <class K, class D>
    struct TreeNodeKey
        {
        // links
        TreeNodeKey<K,D> * Less;
        TreeNodeKey<K,D> * More;
        TreeNodeKey<K,D> * Parent;

        // contents
        K Key;
        D Data;

        // constructor
        TreeNodeKey
            (
            const K & ky,
            const D & item,
            TreeNodeKey<K,D> * sentinel
            );

        TreeNodeKey(); // creates a sentinel node

        // copy constructor
        TreeNodeKey
            (
            const TreeNodeKey<K,D> & node
            );

        // assignment operator
        void operator =
            (
            const TreeNodeKey<K,D> & node
            );
        };

template <class K, class D>
    TreeNodeKey<K,D>::TreeNodeKey
        (
        const K & ky,
        const D & item,
        TreeNodeKey<K,D> * sentinel
        )
        : Data(item), Key(ky)
        {
        Parent = sentinel;
        Less   = sentinel;
        More   = sentinel;
        }

// create sentinel node
template <class K, class D>
    TreeNodeKey<K,D>::TreeNodeKey()
        : Data(), Key()
        {
        Parent = this;
        Less   = this;
        More   = this;
        }

template <class K, class D>
    TreeNodeKey<K,D>::TreeNodeKey
        (
        const TreeNodeKey<K,D> & node
        )
        : Data(node.Data), Key(node.Key)
        {
        Parent = node.Parent;
        Less   = node.Less;
        More   = node.More;
        }

template <class K, class D>
    void TreeNodeKey<K,D>::operator =
        (
        const TreeNodeKey<K,D> & node
        )
        {
        Parent = node.Parent;
        Less   = node.Less;
        More   = node.More;
        Data   = node.Data;
        Key    = node.Key;
        }

//-------------------------------------------------------------
//  BinaryTreeKeyed
//      A basic binary tree, using a key-data relationship
//-------------------------------------------------------------

template <class K, class D>
    class BinaryTreeKeyed
        {
        friend
            class BinaryTreeKeyedIterator<K,D>;

        public:
            // constructor
            BinaryTreeKeyed();

            BinaryTreeKeyed
                (
                const BinaryTreeKeyed<K,D> & tree
                );

            // destructor
            ~BinaryTreeKeyed();

            // assignment opeartor
            void operator =
                (
                const BinaryTreeKeyed<K,D> & tree
                );

            // store an item
            void Insert
                (
                const K & ky,
                const D & item
                );

            // delete an item
            Boolean Delete
                (
                const K & ky
                );

            // look up data associated with a key
            D LookUp
                (
                const K & ky
                );

            // walk entire tree, calling function for nodes
            void Walk
                (
                void (* func)(const K & ky, const D & item)
                );

            // erase all elements in the tree
            void Erase();

            // examine lock count
            unsigned int  GetLockCount();

            // retrieve pointer to sentinel
            const TreeNodeKey<K,D> * GetSentinel();

        protected:
            TreeNodeKey<K,D> * Root;     // root node
            TreeNodeKey<K,D> * Sentinel; // sentinel node

            unsigned int LockCount; // number of iterator locks
            unsigned int Count;

            // function called during traverse
            void (*WalkFunc)(const K & ky, const D & item);

            // function to create and delete nodes
            TreeNodeKey<K,D> * CreateNode
                (
                const K & ky,
                const D & item
                );

            void DeleteNode
                (
                TreeNodeKey<K,D> * node
                );

            // internal insert function
            TreeNodeKey<K,D> * InternalInsert
                (
                const K & ky,
                const D & item
                );

            // recursive copy function
            void RecursiveCopy
                (
                TreeNodeKey<K,D> * node,
                TreeNodeKey<K,D> * sent
                );

            // recursive traversal function
            void RecurseWalk
                (
                TreeNodeKey<K,D> * node
                );

            // recursive deletion function
            void RecursiveDelete
                (
                TreeNodeKey<K,D> * node
                );

            // find minimum node
            TreeNodeKey<K,D> * Minimum
                (
                TreeNodeKey<K,D> * node
                );

            // find maximum node
            TreeNodeKey<K,D> * Maximum
                (
                TreeNodeKey<K,D> * node
                );

            // find successor node
            TreeNodeKey<K,D> * Successor
                (
                TreeNodeKey<K,D> * node
                );

            // find predecessor node
            TreeNodeKey<K,D> * Predecessor
                (
                TreeNodeKey<K,D> * node
                );

            // find node containing specific item
            TreeNodeKey<K,D> * Search
                (
                const K & ky
                );
        };

// constructors
template <class K, class D>
    BinaryTreeKeyed<K,D>::BinaryTreeKeyed()
        {
        Sentinel  = new TreeNodeKey<K,D>;

        if (Sentinel == NULL)
            throw TreeEx(BTX_ALLOC);

        Root      = Sentinel;
        LockCount = 0;
        }

template <class K, class D>
    BinaryTreeKeyed<K,D>::BinaryTreeKeyed
        (
        const BinaryTreeKeyed<K,D> & tree
        )
        {
        Sentinel = new TreeNodeKey<K,D>;

        if (Sentinel == NULL)
            throw TreeEx(BTX_ALLOC);

        Root      = Sentinel;
        LockCount = 0;
        RecursiveCopy(tree.Root, tree.Sentinel);
        }

template <class K, class D>
    BinaryTreeKeyed<K,D>::~BinaryTreeKeyed()
        {
        if (LockCount > 0)
            throw TreeEx(BTX_LOCKVIOLATION);

        RecursiveDelete(Root);

        delete Sentinel;
        }

template <class K, class D>
    void BinaryTreeKeyed<K,D>::operator =
        (
        const BinaryTreeKeyed<K,D> & tree
        )
        {
        if (LockCount > 0)
            throw TreeEx(BTX_LOCKVIOLATION);

        RecursiveDelete(Root);
        Root = Sentinel;
        RecursiveCopy(tree.Root, tree.Sentinel);
        }

// function to create node
template <class K, class D>
    TreeNodeKey<K,D> * BinaryTreeKeyed<K,D>::CreateNode
        (
        const K & ky,
        const D & item
        )
        {
        TreeNodeKey<K,D> * z = new TreeNodeKey<K,D>(ky,item,Sentinel);

        if (z == NULL)
            throw TreeEx(BTX_ALLOC);

        return z;
        }

template <class K, class D>
    void BinaryTreeKeyed<K,D>::DeleteNode
        (
        TreeNodeKey<K,D> * node
        )
        {
        delete node;
        }

// internal insert function
template <class K, class D>
    TreeNodeKey<K,D> * BinaryTreeKeyed<K,D>::InternalInsert
        (
        const K & ky,
        const D & item
        )
        {
        if (LockCount > 0)
            throw TreeEx(BTX_LOCKVIOLATION);

        TreeNodeKey<K,D> * y = Sentinel;
        TreeNodeKey<K,D> * x = Root;

        while (x != Sentinel)
            {
            y = x;

            if (ky == x->Key)
                {
                x->Data = item;
                return NULL;
                }
            else
                {
                if (ky < x->Key)
                    x = x->Less;
                else
                    x = x->More;
                }
            }

        TreeNodeKey<K,D> * z = CreateNode(ky,item);
        z->Parent = y;

        if (y == Sentinel)
            Root = z;
        else
            {
            if (z->Key < y->Key)
                y->Less = z;
            else
                y->More = z;
            }

        return z;
        }

// find minimum node
template <class K, class D>
    TreeNodeKey<K,D> * BinaryTreeKeyed<K,D>::Minimum
        (
        TreeNodeKey<K,D> * node
        )
        {
        if (node != Sentinel)
            while (node->Less != Sentinel)
                node = node->Less;

        return node;
        }

// find maximum node
template <class K, class D>
    TreeNodeKey<K,D> * BinaryTreeKeyed<K,D>::Maximum
        (
        TreeNodeKey<K,D> * node
        )
        {
        if (node != Sentinel)
            while (node->More != Sentinel)
                node = node->More;

        return node;
        }

// find successor node
template <class K, class D>
    TreeNodeKey<K,D> * BinaryTreeKeyed<K,D>::Successor
        (
        TreeNodeKey<K,D> * node
        )
        {
        TreeNodeKey<K,D> * x, * y;

        if (node->More != Sentinel)
            return Minimum(node->More);
        else
            {
            x = node;
            y = node->Parent;

            while ((y != Sentinel) && (x == y->More))
                {
                x = y;
                y = y->Parent;
                }
            }

        return y;
        }

// find predecessor node
template <class K, class D>
    TreeNodeKey<K,D> * BinaryTreeKeyed<K,D>::Predecessor
        (
        TreeNodeKey<K,D> * node
        )
        {
        TreeNodeKey<K,D> * x, * y;

        if (node->Less != Sentinel)
            return Maximum(node->Less);
        else
            {
            x = node;
            y = node->Parent;

            while ((y != Sentinel) && (x == y->Less))
                {
                x = y;
                y = y->Parent;
                }
            }

        return y;
        }

// find node containing specific item
template <class K, class D>
    TreeNodeKey<K,D> * BinaryTreeKeyed<K,D>::Search
        (
        const K & ky
        )
        {
        TreeNodeKey<K,D> * n = Root;

        while ((n != Sentinel) && (n->Key != ky))
            {
            if (ky < n->Key)
                n = n->Less;
            else
                n = n->More;
            }

        return n;
        }

template <class K, class D>
    inline void BinaryTreeKeyed<K,D>::Insert
        (
        const K & ky,
        const D & item
        )
        {
        InternalInsert(ky,item);
        }

template <class K, class D>
    Boolean BinaryTreeKeyed<K,D>::Delete
        (
        const K & ky
        )
        {
        if (LockCount > 0)
            throw TreeEx(BTX_LOCKVIOLATION);

        // find node
        TreeNodeKey<K,D> * z = Search(ky);

        if (z == Sentinel)
            return BOOL_FALSE;

        TreeNodeKey<K,D> * y, * x;

        // find node to splice out
        if ((z->Less == Sentinel) || (z->More == Sentinel))
            y = z;
        else
            y = Successor(z);

        // find child with which to replace y
        if (y->Less != Sentinel)
            x = y->Less;
        else
            x = y->More;

        // splice child onto parent
        if (x != Sentinel)
            x->Parent = y->Parent;

        if (y->Parent == Sentinel)
            Root = x; // replace root
        else
            {
            // splice in child node
            if (y == y->Parent->Less)
                y->Parent->Less = x;
            else
                y->Parent->More = x;
            }

        // if needed, save y data
        if (y != z)
            {
            z->Key  = y->Key;
            z->Data = y->Data;
            }

        // free memory
        DeleteNode(y);

        return BOOL_TRUE;
        }

template <class K, class D>
    D BinaryTreeKeyed<K,D>::LookUp
        (
        const K & ky
        )
        {
        // find node
        TreeNodeKey<K,D> * z = Search(ky);

        if (z == Sentinel)
            throw TreeEx(BTX_NOTFOUND);

        return z->Data;
        }

template <class K, class D>
    void BinaryTreeKeyed<K,D>::Walk
        (
        void (* func)(const K & ky, const D & item)
        )
        {
        WalkFunc = func;
        RecurseWalk(Root);
        }

template <class K, class D>
    void BinaryTreeKeyed<K,D>::Erase()
        {
        if (LockCount > 0)
            throw TreeEx(BTX_LOCKVIOLATION);

        RecursiveDelete(Root);

        Root = Sentinel;
        }

template <class K, class D>
    void BinaryTreeKeyed<K,D>::RecursiveCopy
        (
        TreeNodeKey<K,D> * node,
        TreeNodeKey<K,D> * sent
        )
        {
        if (node != sent)
            {
            Insert(node->Key,node->Data);
            RecursiveCopy(node->Less,sent);
            RecursiveCopy(node->More,sent);
            }
        }

template <class K, class D>
    void BinaryTreeKeyed<K,D>::RecurseWalk
        (
        TreeNodeKey<K,D> * node
        )
        {
        if (node != Sentinel)
            {
            RecurseWalk(node->Less);
            WalkFunc(node->Key,node->Data);
            RecurseWalk(node->More);
            }
        }

template <class K, class D>
    void BinaryTreeKeyed<K,D>::RecursiveDelete
        (
        TreeNodeKey<K,D> * node
        )
        {
        if (node != Sentinel)
            {
            RecursiveDelete(node->Less);
            RecursiveDelete(node->More);
            DeleteNode(node);
            }
        }

// manipulate lock count
template <class K, class D>
    inline size_t BinaryTreeKeyed<K,D>::GetLockCount()
        {
        return LockCount;
        }

template <class K, class D>
    inline const TreeNodeKey<K,D> * BinaryTreeKeyed<K,D>::GetSentinel()
        {
        return Sentinel;
        }

//---------------------------
// binary tree iterator class
//---------------------------

template <class K, class D>
    class BinaryTreeKeyedIterator
        {
        public:
            BinaryTreeKeyedIterator
                (
                BinaryTreeKeyed<K,D> & bt
                );

            BinaryTreeKeyedIterator
                (
                BinaryTreeKeyedIterator<K,D> & iter
                );

            ~BinaryTreeKeyedIterator();

            void operator =
                (
                BinaryTreeKeyedIterator<K,D> & iter
                );

            void Smallest();
            void Largest();

            void operator ++ ();
            void operator -- ();

            D operator * ();
            K GetKey();

        protected:
            BinaryTreeKeyed<K,D> & Tree;
            TreeNodeKey<K,D>   * Node;
        };

template <class K, class D>
    BinaryTreeKeyedIterator<K,D>::BinaryTreeKeyedIterator
        (
        BinaryTreeKeyed<K,D> & bt
        )
        : Tree(bt)
        {
        if (Tree.LockCount == UINT_MAX)
            throw TreeEx(BTX_LOCKMAX);

        ++Tree.LockCount;

        Smallest();
        }

template <class K, class D>
    BinaryTreeKeyedIterator<K,D>::BinaryTreeKeyedIterator
        (
        BinaryTreeKeyedIterator<K,D> & iter
        )
        : Tree(iter.Tree)
        {
        if (Tree.LockCount == UINT_MAX)
            throw TreeEx(BTX_LOCKMAX);

        ++Tree.LockCount;

        Node = iter.Node;
        }

template <class K, class D>
    BinaryTreeKeyedIterator<K,D>::~BinaryTreeKeyedIterator()
        {
        if (Tree.LockCount == 0)
            throw TreeEx(BTX_LOCKZERO);

        --Tree.LockCount;
        }

template <class K, class D>
    void BinaryTreeKeyedIterator<K,D>::operator =
        (
        BinaryTreeKeyedIterator<K,D> & iter
        )
        {
        Tree = iter.Tree;

        if (Tree.LockCount == 0)
            throw TreeEx(BTX_LOCKZERO);

        --Tree.LockCount;

        if (Tree.LockCount == UINT_MAX)
            throw TreeEx(BTX_LOCKMAX);

        ++Tree.LockCount;

        Node = iter.Node;
        }

template <class K, class D>
    void BinaryTreeKeyedIterator<K,D>::Smallest()
        {
        Node = Tree.Minimum(Tree.Root);
        }

template <class K, class D>
    void BinaryTreeKeyedIterator<K,D>::Largest()
        {
        Node = Tree.Maximum(Tree.Root);
        }

template <class K, class D>
    void BinaryTreeKeyedIterator<K,D>::operator ++ ()
        {
        Node = Tree.Successor(Node);
        }

template <class K, class D>
    void BinaryTreeKeyedIterator<K,D>::operator -- ()
        {
        Node = Tree.Predecessor(Node);
        }

template <class K, class D>
    D BinaryTreeKeyedIterator<K,D>::operator * ()
        {
        if (Node == Tree.GetSentinel())
            throw TreeEx(BTX_NOTFOUND);

        return Node->Data;
        }

template <class K, class D>
    K BinaryTreeKeyedIterator<K,D>::GetKey()
        {
        if (Node == Tree.GetSentinel())
            throw TreeEx(BTX_NOTFOUND);

        return Node->Key;
        }

#endif
