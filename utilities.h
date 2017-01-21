#ifndef UTILITIES_HPP
#define UTILITIES_HPP
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
//__declspec( dllexport )

namespace Utilities {
void Reverse(char *pBegin, char *pEnd);
int Partition(int data[], int length, int start, int end);

namespace BiTree {
struct BiTreeNode {
  int m_nValue;
  BiTreeNode *m_pLeft;
  BiTreeNode *m_pRight;
};

BiTreeNode *CreateBiTreeNode(int value);
void SetBiTreeNodeValue(BiTreeNode *pNode, int value);
void ConnectBiTreeNodes(BiTreeNode *pParent, BiTreeNode *pLeft,
                        BiTreeNode *pRight);
void PrintTreeNode(const BiTreeNode *pNode);
void PrintTree(const BiTreeNode *pRoot);
void DestroyTree(BiTreeNode *pRoot);

auto visitBiTreeNode = [](BiTreeNode *root) { printf("%3d", root->m_nValue); };
enum class Order { preOrder, inOrder, postOrder };

void traverseBiTreeRec(BiTreeNode *root, Order order);
}

namespace Tree {
struct TreeNode {
  int m_nValue;
  std::vector<TreeNode *> m_vChildren;
};
TreeNode *CreateTreeNode(int value);
void ConnectTreeNodes(TreeNode *pParent, TreeNode *pChild);
void PrintTreeNode(const TreeNode *pNode);
void PrintTree(const TreeNode *pRoot);
void DestroyTree(TreeNode *pRoot);
}

namespace List {
struct ListNode {
  int m_nValue;
  ListNode *m_pNext;
};

ListNode *CreateListNode(int value);
void ConnectListNodes(ListNode *pCurrent, ListNode *pNext);
void PrintListNode(ListNode *pNode);
void PrintList(ListNode *pHead);
void DestroyList(ListNode *pHead);
void AddToTail(ListNode **pHead, int value);
void RemoveNode(ListNode **pHead, int value);
}
}
#endif // UTILITIES_HPP
