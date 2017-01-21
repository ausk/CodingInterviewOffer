#include "utilities.h"
#include <stdexcept>

namespace Utilities {

void Reverse(char *pBegin, char *pEnd) {
  if (pBegin == nullptr || pEnd == nullptr)
    return;
  while (pBegin < pEnd) {
    char tmp = *pBegin;
    *pBegin = *pEnd;
    *pEnd = tmp;
    ++pBegin, --pEnd;
  }
}

// Random Partition
int RandomInRange(int min, int max) {
  int random = rand() % (max - min + 1) + min;
  return random;
}

void Swap(int *num1, int *num2) {
  int temp = *num1;
  *num1 = *num2;
  *num2 = temp;
}

int Partition(int data[], int length, int start, int end) {
  if (data == nullptr || length <= 0 || start < 0 || end >= length)
    throw new std::runtime_error(std::string("Invalid Parameter!"));

  int index = RandomInRange(start, end);
  Swap(&data[index], &data[end]);

  int small = start - 1;
  for (index = start; index < end; ++index) {
    if (data[index] < data[end]) {
      ++small;
      if (small != index)
        Swap(&data[index], &data[small]);
    }
  }

  ++small;
  Swap(&data[small], &data[end]);

  return small;
}

namespace Tree {

TreeNode *CreateTreeNode(int value) {
  TreeNode *pNode = new TreeNode();
  pNode->m_nValue = value;

  return pNode;
}

void ConnectTreeNodes(TreeNode *pParent, TreeNode *pChild) {
  if (pParent != nullptr) {
    pParent->m_vChildren.push_back(pChild);
  }
}

void PrintTreeNode(const TreeNode *pNode) {
  if (pNode != nullptr) {
    printf("value of this node is: %d.\n", pNode->m_nValue);

    printf("its children is as the following:\n");
    std::vector<TreeNode *>::const_iterator i = pNode->m_vChildren.begin();
    while (i < pNode->m_vChildren.end()) {
      if (*i != nullptr)
        printf("%d\t", (*i)->m_nValue);
    }

    printf("\n");
  } else {
    printf("this node is nullptr.\n");
  }

  printf("\n");
}

void PrintTree(const TreeNode *pRoot) {
  PrintTreeNode(pRoot);

  if (pRoot != nullptr) {
    std::vector<TreeNode *>::const_iterator i = pRoot->m_vChildren.begin();
    while (i < pRoot->m_vChildren.end()) {
      PrintTree(*i);
      ++i;
    }
  }
}

void DestroyTree(TreeNode *pRoot) {
  if (pRoot != nullptr) {
    std::vector<TreeNode *>::iterator i = pRoot->m_vChildren.begin();
    while (i < pRoot->m_vChildren.end()) {
      DestroyTree(*i);
      ++i;
    }

    delete pRoot;
  }
}
}

namespace BiTree {
void traverseBiTreeRec(BiTreeNode *root, Order order) {
  if (root != nullptr) {
    if (order == Order::preOrder) {
      visitBiTreeNode(root);
    }
    traverseBiTreeRec(root->m_pLeft, order);
    if (order == Order::inOrder) {
      visitBiTreeNode(root);
    }
    traverseBiTreeRec(root->m_pRight, order);
    if (order == Order::postOrder) {
      visitBiTreeNode(root);
    }
  } else {
    //叶子节点
    printf("  #");
  }
}

void SetBiTreeNodeValue(BiTreeNode *pNode,int value){
    if(pNode){
        pNode->m_nValue=value;
    }
}

BiTreeNode *CreateBiTreeNode(int value) {
  BiTreeNode *pNode = new BiTreeNode();
  pNode->m_nValue = value;
  pNode->m_pLeft = nullptr;
  pNode->m_pRight = nullptr;

  return pNode;
}

void ConnectBiTreeNodes(BiTreeNode *pParent, BiTreeNode *pLeft,
                      BiTreeNode *pRight) {
  if (pParent != nullptr) {
    pParent->m_pLeft = pLeft;
    pParent->m_pRight = pRight;
  }
}

void PrintTreeNode(const BiTreeNode *pNode) {
  if (pNode != nullptr) {
    printf("value of this node is: %d\n", pNode->m_nValue);

    if (pNode->m_pLeft != nullptr)
      printf("value of its m_pLeft child is: %d.\n", pNode->m_pLeft->m_nValue);
    else
      printf("m_pLeft child is nullptr.\n");

    if (pNode->m_pRight != nullptr)
      printf("value of its right child is: %d.\n", pNode->m_pRight->m_nValue);
    else
      printf("right child is nullptr.\n");
  } else {
    printf("this node is nullptr.\n");
  }

  printf("\n");
}

void PrintTree(const BiTreeNode *pRoot) {
  PrintTreeNode(pRoot);

  if (pRoot != nullptr) {
    if (pRoot->m_pLeft != nullptr)
      PrintTree(pRoot->m_pLeft);

    if (pRoot->m_pRight != nullptr)
      PrintTree(pRoot->m_pRight);
  }
}

void DestroyTree(BiTreeNode *pRoot) {
  if (pRoot != nullptr) {
    BiTreeNode *pLeft = pRoot->m_pLeft;
    BiTreeNode *pRight = pRoot->m_pRight;

    delete pRoot;
    pRoot = nullptr;

    DestroyTree(pLeft);
    DestroyTree(pRight);
  }
}
}

namespace List {
ListNode *CreateListNode(int value) {
  ListNode *pNode = new ListNode();
  pNode->m_nValue = value;
  pNode->m_pNext = nullptr;

  return pNode;
}

void ConnectListNodes(ListNode *pCurrent, ListNode *pNext) {
  if (pCurrent == nullptr) {
    printf("Error to connect two nodes.\n");
    exit(1);
  }

  pCurrent->m_pNext = pNext;
}

void PrintListNode(ListNode *pNode) {
  if (pNode == nullptr) {
    printf("The node is nullptr\n");
  } else {
    printf("The key in node is %d.\n", pNode->m_nValue);
  }
}

void PrintList(ListNode *pHead) {

  ListNode *pNode = pHead;
  if(pNode == nullptr)
      printf("   This is a null list\n");
  while (pNode != nullptr) {
    printf("%5d", pNode->m_nValue);
    pNode = pNode->m_pNext;
  }
  printf("\n");
}

void DestroyList(ListNode *pHead) {
  ListNode *pNode = pHead;
  while (pNode != nullptr) {
    pHead = pHead->m_pNext;
    delete pNode;
    pNode = pHead;
  }
}

void AddToTail(ListNode **pHead, int value) {
  ListNode *pNew = new ListNode();
  pNew->m_nValue = value;
  pNew->m_pNext = nullptr;

  if (*pHead == nullptr) {
    *pHead = pNew;
  } else {
    ListNode *pNode = *pHead;
    while (pNode->m_pNext != nullptr)
      pNode = pNode->m_pNext;

    pNode->m_pNext = pNew;
  }
}

void RemoveNode(ListNode **pHead, int value) {
  if (pHead == nullptr || *pHead == nullptr)
    return;

  ListNode *pToBeDeleted = nullptr;
  if ((*pHead)->m_nValue == value) {
    pToBeDeleted = *pHead;
    *pHead = (*pHead)->m_pNext;
  } else {
    ListNode *pNode = *pHead;
    while (pNode->m_pNext != nullptr && pNode->m_pNext->m_nValue != value)
      pNode = pNode->m_pNext;

    if (pNode->m_pNext != nullptr && pNode->m_pNext->m_nValue == value) {
      pToBeDeleted = pNode->m_pNext;
      pNode->m_pNext = pNode->m_pNext->m_pNext;
    }
  }

  if (pToBeDeleted != nullptr) {
    delete pToBeDeleted;
    pToBeDeleted = nullptr;
  }
}
}
}
