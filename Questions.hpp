#ifndef Q10_HPP
#define Q10_HPP

#include "assert.h"
#include "utilities.h"
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <glog/logging.h>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <map>
#include <memory>
#include <queue>
#include <stack>
#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>
using namespace std;
template <typename T> ostream &operator<<(ostream &os, vector<T> &vec) {
  for (auto item : vec)
    os << item << " ";
  return os;
}
template <typename T> bool myequal(const T item1, const T item2) {
  return true;
}
template <> bool myequal<double>(double item1, double item2) {
  return ((item1 - item2) > -1E-10) && ((item1 - item2) < 1E-10);
}

template <typename T>
bool myequal(const vector<T> &vec1, const vector<T> &vec2) {
  if (vec1.size() != vec2.size())
    return false;
  bool bEqual = true;
  for (typename vector<T>::const_iterator iter1 = vec1.begin(),
                                          iter2 = vec2.begin();
       iter1 != vec1.end(); ++iter1, ++iter2) {
    if (!myequal(*iter1, *iter2)) {
      bEqual = false;
      break;
    }
  }
  return bEqual;
}

namespace CodingInterview {

namespace Q01 {
// 面试题1：赋值运算符函数
// 题目：如下为类型CMyString的声明，请为该类型添加赋值运算符函数。

// P447 Copy Control: 3-5 principles
// 空指针与空字符串，字符串的长度与内存空间。
// 类的拷贝操作的三五法则：拷贝构造函数、拷贝赋值运算符、析构函数.
// 精确控制内存的时候，不要用memset/memcpy？

class CMyString;
ostream &operator<<(ostream &os, const CMyString &rhs);

class CMyString {
public:
  char *m_pdata;
  size_t sz;

public:
  CMyString(const char *pdata = "") {
    if (pdata == nullptr) {
      sz = 0;
      m_pdata = (char *)malloc(sizeof(char) * (sz + 1));
      m_pdata[sz] = '\0';
    } else {
      sz = strlen(pdata);
      m_pdata = (char *)malloc(sizeof(char) * (sz + 1));
      for (int i = 0; i < sz; ++i)
        m_pdata[i] = pdata[i];
    }
    m_pdata[sz] = '\0';

    LOG(INFO) << "call CMyString(const char *pdata = \"\")," << *this << endl;
  }
  CMyString(const CMyString &rhs) {
    m_pdata = NULL;
    sz = rhs.sz;
    m_pdata = (char *)malloc(sizeof(char) * (sz + 1));
    for (int i = 0; i < sz; ++i)
      m_pdata[i] = rhs.m_pdata[i];
    m_pdata[sz] = '\0';
    LOG(INFO) << "call CMyString(const CMyString& rhs), " << *this << endl;
  }
  CMyString &operator=(const CMyString &rhs) {
    if (this != &rhs) {
      free(m_pdata);
      m_pdata = NULL;
      m_pdata = (char *)malloc(sizeof(char) * (sz + 1));
      sz = rhs.sz;
      for (int i = 0; i < sz; ++i)
        m_pdata[i] = rhs.m_pdata[i];
      m_pdata[sz] = '\0';
    }

    LOG(INFO) << "call CMyString& operator=(const CMyString&rhs), " << *this
              << endl;
    return *this;
  }
  ~CMyString() {
    LOG(INFO) << "call ~CMyString()" << endl;
    free(m_pdata);
  }

  friend ostream &operator<<(ostream &os, const CMyString &rhs);
};
ostream &operator<<(ostream &os, const CMyString &rhs) {
  os << "sz=" << rhs.sz << ", strlen=" << strlen(rhs.m_pdata)
     << ", data=" << (rhs.m_pdata ? rhs.m_pdata : "NULL");
  return os;
}

int test() {
  LOG(INFO) << "Testing CMyString..." << endl;
  CMyString cs0, cs1 = "hi,c", cs2("hello,c++"), cs3(cs1), cs4 = cs2;
  LOG(INFO) << "cs0:" << cs0 << endl
            << "cs1:" << cs1 << endl
            << "cs2:" << cs2 << endl
            << "cs3:" << cs3 << endl
            << "cs4:" << cs4 << endl;
  cs0 = "Single Assign!";
  LOG(INFO) << "cs0:" << cs0 << endl;
  cs0 = cs0 = "Self Assign!";
  LOG(INFO) << cs0 << endl;
  cs0 = cs1 = cs2 = "Sequence Assign!";
  LOG(INFO) << "cs0:" << cs0 << endl
            << "cs1:" << cs1 << endl
            << "cs2:" << cs2 << endl;
  return 0;
} //! test
} //! Q01

namespace Q02 {
// 面试题2：实现Singleton模式
// 题目：设计一个类，我们只能生成该类的一个实例。
//使用智能指针和私有构造函数实现单子模式
// http://blog.csdn.net/lanchunhui/article/details/50878505
class Singleton {
public:
  static const int ID = 10;
  static shared_ptr<Singleton> getInstance() {
    if (m_pinstance == nullptr) {
      m_pinstance = shared_ptr<Singleton>(new Singleton);
    }
    cout << "cnt=" << ++cnt << endl;
    return m_pinstance;
  }
  void setX(const int x) { this->x = x; }
  int getX() const { return this->x; }

private:
  int x;
  static int cnt;

private:
  static shared_ptr<Singleton> m_pinstance;
  Singleton() : x(0) { std::cout << "Singleton:Singleton()" << std::endl; }
  Singleton(const Singleton &) = delete;
  void operator=(const Singleton &) = delete;
};
shared_ptr<Singleton> Singleton::m_pinstance =
    nullptr; // make_shared<Singleton>(new Singleton);
int Singleton::cnt = 0;

int test() {
  LOG(INFO) << "Testing Singleton..." << endl;
  Singleton::getInstance()->setX(100);
  LOG(INFO) << Singleton::getInstance()->getX() << endl;
} //! test
} //! Q02

namespace Q03 {
// 面试题4：二维数组中的查找
// 题目：在一个二维数组中，每一行都按照从左到右递增的顺序排序，每一列都按
// 照从上到下递增的顺序排序。请完成一个函数，输入这样的一个二维数组和一个
// 整数，判断数组中是否含有该整数。

//从边界点开始查找，什么叫做边界点，合适的边界点事半功倍
//从左下角或者右上角开始，根据带查找元素与当前元素的大小关系来缩小范围

tuple<int, int> FindInPratiallySortedMatrix(const vector<vector<int>> &mat,
                                            int num) {

  int rows = 0, cols = 0;
  tuple<int, int> res = make_tuple<int, int>(-1, -1);
  if ((rows = mat.size()) >= 0 && (cols = mat[0].size()) >= 0 &&
      num >= mat[0][0] && num <= mat[rows - 1][cols - 1]) {

    int row = 0, col = cols - 1;
    while (row < rows & col >= 0) {
      int curr_num = mat[row][col];
      if (curr_num == num) {
        get<0>(res) = row;
        get<1>(res) = col;
        break;
      }
      if (curr_num > num) {
        --col;
      } else if (curr_num < num) {
        ++row;
      }
    }
  }
  return res;
}

ostream &operator<<(ostream &os, tuple<int, int> tp) {
  os << "(" << get<0>(tp) << ',' << get<1>(tp) << ')';
  return os;
}
ostream &operator<<(ostream &os, vector<vector<int>> &mat) {
  os << setfill(' ');
  for (const vector<int> &vec : mat) {
    for (int i : vec) {
      os << setw(4) << i;
    }
    os << "\n";
  }
  os << setfill(' ');
  return os;
}

void test() {

  LOG(INFO) << "Testing CMyString..." << endl;
  //  1   2   8   9
  //  2   4   9   12
  //  4   7   10  13
  //  6   8   11  15
  vector<vector<int>> mat = {
      {1, 2, 8, 9}, {2, 4, 9, 12}, {4, 7, 10, 13}, {6, 8, 11, 15}};
  LOG(INFO) << "Matrix:\n" << mat << endl;
  LOG(INFO) << "Find 0:" << FindInPratiallySortedMatrix(mat, 0) << endl;
  LOG(INFO) << "Find 1:" << FindInPratiallySortedMatrix(mat, 1) << endl;
  LOG(INFO) << "Find 6:" << FindInPratiallySortedMatrix(mat, 6) << endl;
  LOG(INFO) << "Find 9:" << FindInPratiallySortedMatrix(mat, 9) << endl;
  LOG(INFO) << "Find 10:" << FindInPratiallySortedMatrix(mat, 10) << endl;
  LOG(INFO) << "Find 15:" << FindInPratiallySortedMatrix(mat, 15) << endl;
  LOG(INFO) << "Find 16:" << FindInPratiallySortedMatrix(mat, 16) << endl;
} //! test
} //! Q03

namespace Q04 {

using namespace Utilities::List;

// 面试题6：从尾到头打印链表
// 题目：输入一个链表的头结点，从尾到头反过来打印出每个结点的值。
// 递归的本质是栈结构，使用递归的时候，可能会因为调用层级过深而导致函数调用栈溢出。
// 因此显式使用基于循环的栈结构来代替递归方式，增强代码鲁棒性。

void PrintListReversingly_Iteratively(ListNode *pHead) {
  std::stack<ListNode *> nodes;

  ListNode *pNode = pHead;
  while (pNode != nullptr) {
    nodes.push(pNode);
    pNode = pNode->m_pNext;
  }

  while (!nodes.empty()) {
    pNode = nodes.top();
    printf("%d\t", pNode->m_nValue);
    nodes.pop();
  }
}

void PrintListReversingly_Recursively(ListNode *pHead) {
  if (pHead != nullptr) {
    if (pHead->m_pNext != nullptr) {
      PrintListReversingly_Recursively(pHead->m_pNext);
    }

    printf("%d\t", pHead->m_nValue);
  }
}

void testPrintList(ListNode *pHead) {
  PrintList(pHead);
  PrintListReversingly_Iteratively(pHead);
  printf("\n");
  PrintListReversingly_Recursively(pHead);
  printf("\n");
}

void test() {
  {
    printf("\nTest empty(null) list begins.\n");
    testPrintList(nullptr);
  }
  {
    printf("\nTest single node list begins.\n");
    ListNode *pNode1 = CreateListNode(1);
    testPrintList(pNode1);
    DestroyList(pNode1);
  }
  {
    printf("\nTest3 begins.\n");
    printf("1->2->3->4->5\n");

    ListNode *pNode1 = CreateListNode(1);
    ListNode *pNode2 = CreateListNode(2);
    ListNode *pNode3 = CreateListNode(3);
    ListNode *pNode4 = CreateListNode(4);
    ListNode *pNode5 = CreateListNode(5);

    ConnectListNodes(pNode1, pNode2);
    ConnectListNodes(pNode2, pNode3);
    ConnectListNodes(pNode3, pNode4);
    ConnectListNodes(pNode4, pNode5);

    testPrintList(pNode1);
    DestroyList(pNode1);
  }
} //! test
} //! Q04

namespace Q05 {
// 面试题7：重建二叉树
// 题目：输入某二叉树的前序遍历和中序遍历的结果，请重建出该二叉树。假设输
// 入的前序遍历和中序遍历的结果中都不含重复的数字。例如输入前序遍历序列{1,
// 2, 4, 7, 3, 5, 6, 8}和中序遍历序列{4, 7, 2, 1, 5, 3, 8, 6}，则重建出
// 图2.6所示的二叉树并输出它的头结点。
using namespace Utilities::BiTree;

BiTreeNode *ConstructCore(int *startPreorder, int *endPreorder,
                          int *startInorder, int *endInorder);

BiTreeNode *Construct(int *preorder, int *inorder, int length) {
  if (preorder == nullptr || inorder == nullptr || length <= 0)
    return nullptr;

  return ConstructCore(preorder, preorder + length - 1, inorder,
                       inorder + length - 1);
}

BiTreeNode *ConstructCore(int *startPreorder, int *endPreorder,
                          int *startInorder, int *endInorder) {
  // 前序遍历序列的第一个数字是根结点的值
  int rootValue = startPreorder[0];
  BiTreeNode *root = new BiTreeNode();
  root->m_nValue = rootValue;
  root->m_pLeft = root->m_pRight = nullptr;

  if (startPreorder == endPreorder) {
    if (startInorder == endInorder && *startPreorder == *startInorder)
      return root;
    else
      throw std::runtime_error(std::string("Invalid input."));
  }

  // 在中序遍历中找到根结点的值
  int *rootInorder = startInorder;
  while (rootInorder <= endInorder && *rootInorder != rootValue)
    ++rootInorder;

  if (rootInorder == endInorder && *rootInorder != rootValue)
    throw std::runtime_error(std::string("Invalid input."));

  int leftLength = rootInorder - startInorder;
  int *leftPreorderEnd = startPreorder + leftLength;
  if (leftLength > 0) {
    // 构建左子树
    root->m_pLeft = ConstructCore(startPreorder + 1, leftPreorderEnd,
                                  startInorder, rootInorder - 1);
  }
  if (leftLength < endPreorder - startPreorder) {
    // 构建右子树
    root->m_pRight = ConstructCore(leftPreorderEnd + 1, endPreorder,
                                   rootInorder + 1, endInorder);
  }

  return root;
}

// ====================测试代码====================
void testBiTreeRebuild(char *testName, int *preorder, int *inorder,
                       int length) {
  if (testName != nullptr)
    printf("%s begins:\n", testName);

  printf("The preorder sequence is: ");
  for (int i = 0; i < length; ++i)
    printf("%d ", preorder[i]);
  printf("\n");

  printf("The inorder sequence is: ");
  for (int i = 0; i < length; ++i)
    printf("%d ", inorder[i]);
  printf("\n");

  try {
    BiTreeNode *root = Construct(preorder, inorder, length);
    PrintTree(root);

    DestroyTree(root);
  } catch (std::exception &exception) {
    printf("Invalid Input.\n");
  }
}

int test() {
  {
    // 输入空指针
    testBiTreeRebuild("Test null/empty tree", nullptr, nullptr, 0);
  }

  {
    const int length = 1;
    int preorder[length] = {1};
    int inorder[length] = {1};

    testBiTreeRebuild("Test single node tree", preorder, inorder, length);
  }
  {
    // 输入的两个序列不匹配
    const int length = 7;
    int preorder[length] = {1, 2, 4, 5, 3, 6, 7};
    int inorder[length] = {4, 2, 8, 1, 6, 3, 7};

    testBiTreeRebuild("Test unmatched input", preorder, inorder, length);
  }
  {
    const int length = 5;
    int preorder[length] = {1, 2, 3, 4, 5};
    int inorder[length] = {5, 4, 3, 2, 1};

    testBiTreeRebuild("Test m_pLeft hand tree", preorder, inorder, length);
  }
  {
    const int length = 5;
    int preorder[length] = {1, 2, 3, 4, 5};
    int inorder[length] = {1, 2, 3, 4, 5};

    testBiTreeRebuild("Test right hand tree", preorder, inorder, length);
  }
  {
    // 普通二叉树
    //              1
    //           /     \
    //          2       3
    //         /       / \
    //        4       5   6
    //         \         /
    //          7       8

    const int length = 8;
    int preorder[length] = {1, 2, 4, 7, 3, 5, 6, 8};
    int inorder[length] = {4, 7, 2, 1, 5, 3, 8, 6};

    testBiTreeRebuild("Test normal binary tree", preorder, inorder, length);
  }

  {
    // 完全二叉树
    //              1
    //           /     \
    //          2       3
    //         / \     / \
    //        4   5   6   7

    const int length = 7;
    int preorder[length] = {1, 2, 4, 5, 3, 6, 7};
    int inorder[length] = {4, 2, 5, 1, 6, 3, 7};

    testBiTreeRebuild("Test full binary tree", preorder, inorder, length);
  }

  return 0;
} //! test
} //! Q05

namespace Q06 {
// 面试题8：二叉树的下一个结点
// 题目：给定一棵二叉树和其中的一个结点，如何找出中序遍历顺序的下一个结点？
// 树中的结点除了有两个分别指向左右子结点的指针以外，还有一个指向父结点的指针。

struct BiTreeNode {
  int m_nValue;
  BiTreeNode *m_pLeft;
  BiTreeNode *m_pRight;
  BiTreeNode *m_pParent;
};

BiTreeNode *GetNext(BiTreeNode *pNode) {
  if (pNode == nullptr)
    return nullptr;

  BiTreeNode *pNext = nullptr;
  if (pNode->m_pRight != nullptr) {
    BiTreeNode *pRight = pNode->m_pRight;
    while (pRight->m_pLeft != nullptr)
      pRight = pRight->m_pLeft;

    pNext = pRight;
  } else if (pNode->m_pParent != nullptr) {
    BiTreeNode *pCurrent = pNode;
    BiTreeNode *pParent = pNode->m_pParent;
    while (pParent != nullptr && pCurrent == pParent->m_pRight) {
      pCurrent = pParent;
      pParent = pParent->m_pParent;
    }

    pNext = pParent;
  }

  return pNext;
}

// ==================== 辅助代码用来构建二叉树 ====================
BiTreeNode *CreateBiTreeNode(int value) {
  BiTreeNode *pNode = new BiTreeNode();
  pNode->m_nValue = value;
  pNode->m_pLeft = nullptr;
  pNode->m_pRight = nullptr;
  pNode->m_pParent = nullptr;

  return pNode;
}

void ConnectTreeNodes(BiTreeNode *pParent, BiTreeNode *pLeft,
                      BiTreeNode *pRight) {
  if (pParent != nullptr) {
    pParent->m_pLeft = pLeft;
    pParent->m_pRight = pRight;

    if (pLeft != nullptr)
      pLeft->m_pParent = pParent;
    if (pRight != nullptr)
      pRight->m_pParent = pParent;
  }
}

void PrintTreeNode(BiTreeNode *pNode) {
  if (pNode != nullptr) {
    printf("value of this node is: %d\n", pNode->m_nValue);

    if (pNode->m_pLeft != nullptr)
      printf("value of its m_pLeft child is: %d.\n", pNode->m_pLeft->m_nValue);
    else
      printf("m_pLeft child is null.\n");

    if (pNode->m_pRight != nullptr)
      printf("value of its right child is: %d.\n", pNode->m_pRight->m_nValue);
    else
      printf("right child is null.\n");
  } else {
    printf("this node is null.\n");
  }

  printf("\n");
}

void PrintTree(BiTreeNode *pRoot) {
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

// ====================测试代码====================
void testBiTree(char *testName, BiTreeNode *pNode, BiTreeNode *expected) {
  if (testName != nullptr)
    printf("%s begins: ", testName);

  BiTreeNode *pNext = GetNext(pNode);
  if (pNext == expected)
    printf("Passed.\n");
  else
    printf("FAILED.\n");
}

int test() {
  {
    BiTreeNode *pNode5 = CreateBiTreeNode(5);
    testBiTree("Test Single Node", pNode5, nullptr);
    DestroyTree(pNode5);
  }
  {
    //           8
    //        6    10
    //       5 7  9  11
    BiTreeNode *pNode8 = CreateBiTreeNode(8);
    BiTreeNode *pNode6 = CreateBiTreeNode(6);
    BiTreeNode *pNode10 = CreateBiTreeNode(10);
    BiTreeNode *pNode5 = CreateBiTreeNode(5);
    BiTreeNode *pNode7 = CreateBiTreeNode(7);
    BiTreeNode *pNode9 = CreateBiTreeNode(9);
    BiTreeNode *pNode11 = CreateBiTreeNode(11);

    ConnectTreeNodes(pNode8, pNode6, pNode10);
    ConnectTreeNodes(pNode6, pNode5, pNode7);
    ConnectTreeNodes(pNode10, pNode9, pNode11);

    testBiTree("Test1", pNode8, pNode9);
    testBiTree("Test2", pNode6, pNode7);
    testBiTree("Test3", pNode10, pNode11);
    testBiTree("Test4", pNode5, pNode6);
    testBiTree("Test5", pNode7, pNode8);
    testBiTree("Test6", pNode9, pNode10);
    testBiTree("Test7", pNode11, nullptr);

    DestroyTree(pNode8);
  }
} //! test
} //! Q06

namespace Q07 {
// 面试题9：用两个栈实现队列
// 题目：用两个栈实现一个队列。队列的声明如下，请实现它的两个函数appendTail
// 和deleteHead，分别完成在队列尾部插入结点和在队列头部删除结点的功能。

template <typename T> class CQueue {
public:
  CQueue(void) {}
  ~CQueue(void) {}

  // 在队列末尾添加一个结点
  void appendTail(const T &node) { stack1.push(node); }

  // 删除队列的头结点
  T deleteHead();

private:
  stack<T> stack1;
  stack<T> stack2;
};

template <typename T> T CQueue<T>::deleteHead() {
  if (stack2.size() <= 0) {
    while (stack1.size() > 0) {
      T &data = stack1.top();
      stack1.pop();
      stack2.push(data);
    }
  }

  if (stack2.size() == 0) {
    throw new std::runtime_error(std::string("queue is empty"));
  }

  T head = stack2.top();
  stack2.pop();

  return head;
}

void Test(char actual, char expected) {
  if (actual == expected)
    printf("Test passed.\n");
  else
    printf("Test failed.\n");
}

int test() {
  CQueue<char> queue;

  queue.appendTail('a');
  queue.appendTail('b');
  queue.appendTail('c');

  char head = queue.deleteHead();
  Test(head, 'a');

  head = queue.deleteHead();
  Test(head, 'b');

  queue.appendTail('d');
  head = queue.deleteHead();
  Test(head, 'c');

  queue.appendTail('e');
  head = queue.deleteHead();
  Test(head, 'd');

  head = queue.deleteHead();
  Test(head, 'e');

  return 0;
} //! test
} //! Q07

namespace Q08 {
// 面试题10：斐波那契数列
// 题目：写一个函数，输入n，求斐波那契（Fibonacci）数列的第n项。

long long FibonacciAns1(unsigned int n) {
  if (n <= 0)
    return 0;
  if (n == 1)
    return 1;
  return FibonacciAns1(n - 1) + FibonacciAns1((n - 2));
}

long long FibonacciAns2(unsigned int n) {
  int result[2] = {0, 1};
  if (n < 2)
    return result[n];

  long long fibNMinusOne = 1;
  long long fibNMinusTwo = 0;
  long long fibN = 0;
  for (unsigned int i = 2; i <= n; ++i) {
    fibN = fibNMinusOne + fibNMinusTwo;

    fibNMinusTwo = fibNMinusOne;
    fibNMinusOne = fibN;
  }

  return fibN;
}

template <unsigned int n> struct FibonacciAns3 {
  enum Value { N = FibonacciAns3<n - 1>::N + FibonacciAns3<n - 2>::N };
};
template <> struct FibonacciAns3<1> {
  enum Value { N = 1 };
};
template <> struct FibonacciAns3<0> {
  enum Value { N = 0 };
};

int test() {
  const unsigned int n = 20;
  cout << FibonacciAns1(n) << endl;
  cout << FibonacciAns2(n) << endl;
  cout << FibonacciAns3<n>::N << endl;
} //! test
} //! Q08

namespace Q09 {
// 面试题64：求1+2+…+n
// 题目：求1+2+…+n，要求不能使用乘除法、for、while、if、else、switch、case
// 等关键字及条件判断语句（A?B:C）。
//////////////////////////////////////////
class SumTemp {
public:
  SumTemp() {
    ++N;
    Sum += N;
  }
  static void reset() { N = 0, Sum = 0; }
  static long long getSum() { return Sum; }

private:
  static unsigned int N;
  static long long Sum;
};
unsigned int SumTemp::N = 0;
long long SumTemp::Sum = 0;

//////////////////////////////////////////
typedef unsigned int (*fun)(unsigned int n);
unsigned int SumFun_Terminator(unsigned int n) { return 0; }
unsigned int SumFun(unsigned int n) {
  static fun f[2] = {SumFun_Terminator, SumFun};
  return n + f[!!n](n - 1);
}

//////////////////////////////////////////
class SumBase;
static SumBase *Array[2];

class SumBase {
public:
  virtual unsigned int Sum(unsigned int n) { return 0; }
};
class SumDerived : public SumBase {
public:
  virtual unsigned int Sum(unsigned int n) {
    return Array[!!n]->Sum(n - 1) + n;
  }
};

template <unsigned int n> struct SumTemplate {
  enum Value { N = n + SumTemplate<n - 1>::N };
};
template <> struct SumTemplate<0> {
  enum Value { N = 0 };
};

int test() {
  const unsigned int n = 20;
  {

    SumTemp::reset();
    SumTemp *pTemp = new SumTemp[n];
    delete[] pTemp;
    pTemp = nullptr;
    cout << SumTemp::getSum() << endl;
  }
  { cout << SumFun(n) << endl; }
  {
    SumBase b;
    SumDerived d;
    Array[0] = &b;
    Array[1] = &d;
    cout << Array[1]->Sum(n) << endl;
  }

  { cout << SumTemplate<n>::N << endl; }
} //! test
} //! Q09

namespace Q10 {

// 面试题67：把字符串转换成整数
// 题目：请你写一个函数StrToInt，实现把字符串转换成整数这个功能。当然，不
// 能使用atoi或者其他类似的库函数。

//有效的最大正整数, 0x7FFFFFFF
//有效的最小负整数, 0x80000000

long long strToInt(const char *str, int &gStatus) {
  gStatus = 0;
  if (str == nullptr) {
    gStatus = -1;
    return 0;
  }
  if (strlen(str) == 0) {
    gStatus = -1;
    return 0;
  }
  bool minus = false;
  const char *digit = str;
  if (*digit == '-') {
    minus = true;
    ++digit;
  } else if (*digit == '+') {
    minus = false;
    ++digit;
  }
  unsigned num = 0;
  while (*digit != '\0') {
    if (*digit >= '0' && *digit <= '9') {
      num = num * 10 + (*digit - '0');
      if ((!minus && num > 0x7FFFFFFF) || (minus && num > 0x80000000)) {
        num = 0;
        gStatus = -3; // overflow
        break;
      }
      ++digit;
    } else {
      num = 0;
      gStatus = -4; // Invalid
      break;
    }
  }

  if (*digit != '\0') {
    num = 0;
    gStatus = -4;
  }
  num = minus ? -num : num;
  return num;
}
void Test(const char *string) {
  int gStatus;
  int result = strToInt(string, gStatus);
  if (gStatus != 0)
    printf("the input %s is invalid.\n", string);
  else
    printf("number for %s is: %d.\n", string, result);
}
int test() {
  Test(nullptr);
  Test("");
  Test("+");
  Test("-");
  Test("+0");
  Test("+123");
  Test("-0");
  Test("-123");

  Test("+1ab");
  Test("-1ab");

  Test("+2147483647");
  Test("+2147483648");
  Test("+2147483649");

  Test("-2147483647");
  Test("-2147483648");
  Test("-2147483649");

  return 0;
} //! test

} //! Q10

namespace Q11 {
// 面试题11：旋转数组的最小数字
// 题目：把一个数组最开始的若干个元素搬到数组的末尾，我们称之为数组的旋转。
// 输入一个递增排序的数组的一个旋转，输出旋转数组的最小元素。例如数组
// {3, 4, 5, 1, 2}为{1, 2, 3, 4, 5}的一个旋转，该数组的最小值为1。

int findMinInRotatedArray(const vector<int> &vec) {
  if (vec.size() <= 0)
    return -1;
  int idx = 0, low = 0, high = vec.size() - 1;

  //注意循环不变量，和终止条件
  while (vec[low] >= vec[high]) {
    // 如果index1和index2指向相邻的两个数，
    // 则index1指向第一个递增子数组的最后一个数字，
    // index2指向第二个子数组的第一个数字，也就是数组中的最小数字
    if (high - low == 1) {
      idx = high;
      break;
    }
    int mid = (high + low) / 2;

    // 如果下标为index1、index2和indexMid指向的三个数字相等，则只能顺序查找
    if (vec[mid] == vec[low] && vec[mid] == vec[high]) {
      idx = low;
      for (int i = low; i <= high; ++i) {
        if (vec[i] < vec[idx])
          idx = i;
      }
      break;
    }
    // 缩小查找范围
    if (vec[mid] < vec[low])
      high = mid;
    else if (vec[mid] > vec[high])
      low = mid;
  }
  return idx;
}
int Test(const vector<int> &vec) {
  copy(vec.begin(), vec.end(), ostream_iterator<int>(cout, " "));
  cout << endl;
  int idx = findMinInRotatedArray(vec);
  cout << "Finded minidx=" << idx << ", minval=" << vec[idx] << endl;
  return 0;
}

int test() {
  cout << "Start testing Q11..." << endl;
  {
    vector<int> vec = {1, 3};
    Test(vec);
  }
  {
    vector<int> vec = {3, 1};
    Test(vec);
  }

  {
    // 有重复数字，并且重复的数字刚好的最小的数字
    vector<int> vec = {3, 1, 1, 2};
    Test(vec);
  }
  {
    // 有重复数字，并且重复的数字刚好的最小的数字
    vector<int> vec = {3, 4, 1, 1, 2};
    Test(vec);
  }

  {
    vector<int> vec = {1, 1, 1, 0, 1};
    Test(vec);
  }
} //! test
} //! Q11

namespace Q12 {
// 面试题12：矩阵中的路径
// 题目：请设计一个函数，用来判断在一个矩阵中是否存在一条包含某字符串所有
// 字符的路径。路径可以从矩阵中任意一格开始，每一步可以在矩阵中向左、右、
// 上、下移动一格。如果一条路径经过了矩阵的某一格，那么该路径不能再次进入
// 该格子。例如在下面的3×4的矩阵中包含一条字符串“bfce”的路径（路径中的字
// 母用下划线标出）。但矩阵中不包含字符串“abfb”的路径，因为字符串的第一个
// 字符b占据了矩阵中的第一行第二个格子之后，路径不能再次进入这个格子。
// A B T G
// C F C S
// J D E H

template <typename T> ostream &operator<<(ostream &os, const vector<T> &vec) {
  for (auto v : vec)
    os << setw(2) << v;
  return os;
}
template <typename T>
ostream &operator<<(ostream &os, const vector<vector<T>> &mat) {
  for (auto vec : mat)
    os << vec << '\n';
  return os;
}

bool hasPathCore(const vector<vector<char>> &words, const vector<char> &str,
                 int row, int col, int curr, vector<vector<bool>> &visited,
                 const int rows, const int cols, const int len) {

  //有效性判断
  if (row >= 0 & row < rows && col >= 0 && col < cols && curr < len &&
      !visited[row][col] && words[row][col] == str[curr]) {
    visited[row][col] = 1; //标记
    if (curr == len - 1) {
      //终止条件

      cout << "\n\nFound it:\n";
      for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
          cout << (visited[i][j] == true ? words[i][j] : '#') << " ";
        }
        cout << endl;
      }
      return true;
    }
    bool bHasPath = (hasPathCore(words, str, row - 1, col, curr + 1, visited,
                                 rows, cols, len) ||
                     hasPathCore(words, str, row + 1, col, curr + 1, visited,
                                 rows, cols, len) ||
                     hasPathCore(words, str, row, col - 1, curr + 1, visited,
                                 rows, cols, len) ||
                     hasPathCore(words, str, row, col + 1, curr + 1, visited,
                                 rows, cols, len));
    visited[row][col] = 0; //回溯
    return bHasPath;

  } else {
    return false;
  }
}

bool hasPath(const vector<vector<char>> &words, const vector<char> &str) {
  int rows = 0, cols = 0, len = 0;
  if ((rows = words.size()) == 0 || (cols = words[0].size()) == 0 ||
      (len = str.size()) > rows * cols)
    return false;
  int row = 0, col = 0, curr = 0;

  // http://www.cnblogs.com/wei-li/archive/2012/06/08/2541576.html
  // C++ vector多维数组初始化及清零
  vector<vector<bool>> visited(rows, vector<bool>(cols, false));

  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      if (hasPathCore(words, str, i, j, curr, visited, rows, cols, len))
        return true;
    }
  }
  return false;
}

int Test(const vector<vector<char>> &words, const vector<char> &str) {
  cout << "\n================================================\n";
  cout << "Matrix:\n" << words << endl;
  cout << "str:\n" << str << endl;
  cout << "Is there str in the matrix?" << boolalpha << hasPath(words, str)
       << noboolalpha << "\n\n";
}

int test() {
  cout << "Start testing Q12..." << endl;
  {
    vector<vector<char>> words = {{'A', 'B', 'C', 'D', 'E'},
                                  {'F', 'G', 'H', 'I', 'J'},
                                  {'K', 'L', 'M', 'N', 'O'},
                                  {'P', 'Q', 'R', 'S', 'T'},
                                  {'U', 'V', 'W', 'X', 'Y'}};
    cout << "Matrix:\n" << words << endl;
    {
      vector<char> str = {'C', 'H', 'M'};
      Test(words, str);
    }
    {
      vector<char> str = {'A', 'B', 'G'};
      Test(words, str);
    }

    {
      vector<char> str = {'A', 'B', 'G', 'F'};
      Test(words, str);
    }
    {
      vector<char> str = {'A', 'B', 'G', 'L'};
      Test(words, str);
    }
    {
      vector<char> str = {'A', 'B', 'G', 'H'};
      Test(words, str);
    }
    {
      vector<char> str = {'A', 'B', 'G', 'B'};
      Test(words, str);
    }
    {
      vector<char> str = {'G', 'H', 'I', 'N', 'S', 'R', 'Q', 'L', 'M'};
      Test(words, str);
    }
    {
      vector<char> str = {'A', 'B', 'C', 'D', 'E', 'J', 'O', 'T', 'Y',
                          'X', 'W', 'V', 'U', 'P', 'K', 'F', 'G', 'H',
                          'I', 'N', 'S', 'R', 'Q', 'L', 'M'};
      Test(words, str);
    }
  }
  {
    vector<vector<char>> words = {
        {'A', 'B', 'C'}, {'A', 'B', 'C'}, {'A', 'B', 'C'}};
    cout << "Matrix:\n" << words << endl;
    {
      vector<char> str = {'A', 'B', 'C'};
      cout << "str:\n" << str << endl;
      cout << hasPath(words, str) << endl;
      Test(words, str);
    }
    {
      vector<char> str = {'A', 'B', 'B', 'B', 'C'};
      Test(words, str);
    }
    {
      vector<char> str = {'A', 'A', 'A', 'B', 'B', 'B', 'C', 'C', 'C'};
      Test(words, str);
    }
    {
      vector<char> str = {'A', 'B', 'C', 'A', 'B', 'C'};
      Test(words, str);
    }
    {
      vector<char> str = {'A', 'B', 'C', 'C', 'B', 'A'};
      Test(words, str);
    }
    {
      vector<char> str = {'A', 'B', 'C', 'B', 'A'};
      Test(words, str);
    }
  }

  return 0;
} //! test
} //! Q12

namespace Q13 {
// 面试题14：剪绳子
// 题目：给你一根长度为n绳子，请把绳子剪成m段（m、n都是整数，n>1并且m≥1）。
// 每段的绳子的长度记为k[0]、k[1]、……、k[m]。k[0]*k[1]*…*k[m]可能的最大乘
// 积是多少？例如当绳子的长度是8时，我们把它剪成长度分别为2、3、3的三段，此
// 时得到最大的乘积18。

// ====================动态规划====================
//求动态规划的转移函数，一般是从下至上，bottom-to-top
int maxProductAfterCutting(const int length) {
  if (length < 1)
    return 0;
  if (length == 1)
    return 1;
  if (length == 2)
    return 2;
  if (length == 3)
    return 3;
  vector<int> product(size_t(length + 1));
  product[0] = 0;
  product[1] = 1;
  product[2] = 2;
  product[3] = 3;

  for (int i = 4; i <= length; ++i) {
    int maxval = 0;
    for (int j = 1; j <= i / 2; ++j) {
      int prod = product[j] * product[i - j];
      if (maxval < prod) {
        maxval = prod;
      }
    }
    product[i] = maxval;
  }
  return product[length];
}
int Test(const char *testName, int length, int expected) {
  cout << testName << ": " << boolalpha
       << (maxProductAfterCutting(length) == expected) << endl;
}

int test() {
  cout << "Start testing Q13..." << endl;
  Test("test0", 0, 0);
  Test("test1", 1, 1);
  Test("test2", 2, 2);
  Test("test3", 3, 3);
  Test("test5", 5, 6);
  Test("test6", 6, 9);
  Test("test7", 7, 12);
  Test("test8", 50, 86093442);
  return 0;
} //! test
} //! Q13

namespace Q14 {
// 面试题15：二进制中1的个数
// 题目：请实现一个函数，输入一个整数，输出该数二进制表示中1的个数。例如
// 把9表示成二进制是1001，有2位是1。因此如果输入9，该函数输出2。

int numOf1InBits(int n) {

  //一定要注意整数溢出的问题
  unsigned int flag = 1;

  int count = 0;
  while (flag) {
    if (flag & n)
      ++count;
    flag <<= 1;
  }
  return count;
}
int Test(const char *testName, int n, int expected) {
  cout << testName << ": " << boolalpha << (numOf1InBits(n) == expected)
       << endl;
}
int test() {
  cout << "Start testing Q14..." << endl;
  // 输入0，期待的输出是0
  Test("test1", 0, 0);

  // 输入1，期待的输出是1
  Test("test2", 1, 1);

  // 输入10，期待的输出是2
  Test("test3", 10, 2);

  // 输入0x7FFFFFFF，期待的输出是31
  Test("test4", 0x7FFFFFFF, 31);

  // 输入0xFFFFFFFF（负数），期待的输出是32
  Test("test5", 0xFFFFFFFF, 32);

  // 输入0x80000000（负数），期待的输出是1
  Test("test6", 0x80000000, 1);

  return 0;
} //! test
} //! Q14

namespace Q15 {

// 面试题16：数值的整数次方
// 题目：实现函数double Power(double base, int exponent)，求base的exponent
// 次方。不得使用库函数，同时不需要考虑大数问题。
//对输入进行分类汇总，当指数为正、负、零时，分别对应不同情况
//指数为令，返回1；指数为负，则底数不为令，返回指数为正时的倒数
//计算指数为正的次幂的时候，根据奇偶性，计算连乘

double powerWithUExp(double base, unsigned int exp) {
  if (exp == 0)
    return 1;
  else if (exp == 1)
    return base;

  //注意递归结构
  double res = powerWithUExp(base, exp >> 1);
  res *= res;
  if ((exp & 0x01) == 1)
    res *= base;
  return res;
}

double power(double base, int exp) {
  if (exp == 0)
    return 1;
  bool negexp = false;
  if (exp < 0) {
    if (myequal<double>(base, 1E-10))
      throw new std::logic_error(std::string("Cann't divide by zero!"));
    exp = -exp;
    negexp = true;
  }
  double prod = powerWithUExp(base, exp);
  return negexp ? (1.0 / prod) : (prod);
}

int Test(const char *testName, double num, int exp, double expected) {
  try {
    double prod = power(num, exp);
    cout << testName << ":" << boolalpha << myequal<double>(prod, expected)
         << endl
         << "x^y=" << num << "^" << exp << "=" << prod << endl
         << "expected=" << expected << endl
         << flush << endl;

  } catch (std::logic_error e) {
    cout << "Error:" << e.what() << endl;
  }
}

int test() {
  // Test("test0", 0, -1, 1); // Error
  Test("test1", 2, 0, 1);
  Test("test2", 2, 3, 8);
  Test("test3", 2, -3, 0.125);
  Test("test4", -2, 0, 1);
  Test("test5", -2, -2, 0.25);
  Test("test6", -2, 2, 4);
  Test("test7", -2, -3, -0.125);
  Test("test8", -2, 3, -8);

  Test("test11", 1, 0, 1);
  Test("test12", -1, 0, 1);
  Test("test13", 0, 0, 1);
  Test("test14", 0, 1, 0);
  Test("test21", 2, 31, 2147483648);
  Test("test22", 2, -31, 4.656612873077393e-10);
  Test("test21", 2, 15, 32768);
  Test("test22", 2, -15, 3.0517578125e-05);
} //! test
} //! Q15

namespace Q16 {
// 面试题17：打印1到最大的n位数
// 题目：输入数字n，按顺序打印出从1最大的n位十进制数。比如输入3，则
// 打印出1、2、3一直到最大的3位数即999。

ostream &operator<<(ostream &os, vector<int> &vec) {
  bool zflag = true;
  for (int i = 0; i < vec.size() - 1; ++i) {
    if (vec[i] != 0)
      zflag = false;
    os << (!zflag ? char(vec[i] + '0') : ' ');
  }
  os << vec[vec.size() - 1];
  return os;
}

int printNum(unsigned int n, const unsigned int base, vector<int> &vec) {
  static int cnt = 0;
  static vector<char> table = {0, 1, 2,   3,   4,   5,   6,   7,
                               8, 9, 'A', 'B', 'C', 'D', 'E', 'F'};
  // 模拟中序遍历
  // unsigned int 注意防止溢出
  // 辅助代码比核心代码还要多？？？
  if (n > 0) {
    for (int b = 0; b < base; ++b) {
      vec[vec.size() - n] = table[b];
      if (n != 1) {
        printNum(n - 1, base, vec);
      } else {
        //查找叶子节点，输出
        LOG(INFO) << setw(4) << cnt << " : " << vec << endl << flush;
        cout << setw(4) << cnt << " : " << vec << endl << flush;
        ++cnt;
      }
    }
  }
}
int printNum(unsigned int n) {
  if (n > 0) {
    vector<int> vec(n, 0);
    printNum(n, 10, vec);
  }
}
int test() {
  printNum(2);
  vector<int> vec = {0, 1, 2, 3, 4, 5};
  cout << vec << endl;
} //! test
} //! Q16

namespace Q17 {
// 面试题18（一）：在O(1)时间删除链表结点
// 题目：给定单向链表的头指针和一个结点指针，定义一个函数在O(1)时间删除该结点。
// void DeleteNode(ListNode** pListHead, ListNode* pToBeDeleted);
using namespace Utilities::List;
void DeleteNode(ListNode **pListHead, ListNode *pToBeDeleted) {
  //输入链表的有效性
  if (!pListHead || !(*pListHead) || !pToBeDeleted) {
    return;
  }
  // TODO:注意寻找外援，也就是各种临时变量、哨兵
  // 要删除的结点不是尾结点
  // 头结点，尾节点，中间结点
  if (pToBeDeleted->m_pNext != nullptr) {
    ListNode *pNext = pToBeDeleted->m_pNext;
    pToBeDeleted->m_nValue = pNext->m_nValue;
    pToBeDeleted->m_pNext = pNext->m_pNext;

    delete pNext;
    pNext = nullptr;
  }
  // 链表只有一个结点，删除头结点（也是尾结点）
  else if (*pListHead == pToBeDeleted) {
    delete pToBeDeleted;
    pToBeDeleted = nullptr;
    *pListHead = nullptr;
  }
  // 链表中有多个结点，删除尾结点
  else {
    ListNode *pNode = *pListHead;
    while (pNode->m_pNext != pToBeDeleted) {
      pNode = pNode->m_pNext;
    }

    pNode->m_pNext = nullptr;
    delete pToBeDeleted;
    pToBeDeleted = nullptr;
  }
}
// ====================测试代码====================
void Test(const char *testName, ListNode *pListHead, ListNode *pNode) {
  cout << "\n========================================\n" << testName << endl;
  cout << "The original list is: \n";
  PrintList(pListHead);

  cout << "The node to be deleted is: \n";
  PrintListNode(pNode);

  DeleteNode(&pListHead, pNode);

  cout << "The result list is: \n";
  PrintList(pListHead);
  cout << "\n\n\n";
}

void test() {
  { Test("Test null", nullptr, nullptr); }
  {
    ListNode *pNode1 = CreateListNode(1);
    Test("Test singlenode", pNode1, pNode1);
  }
  {
    // 链表中有多个结点，删除尾结点\中间的结点\头结点
    ListNode *pNode1 = CreateListNode(1);
    ListNode *pNode2 = CreateListNode(2);
    ListNode *pNode3 = CreateListNode(3);
    ListNode *pNode4 = CreateListNode(4);
    ListNode *pNode5 = CreateListNode(5);

    ConnectListNodes(pNode1, pNode2);
    ConnectListNodes(pNode2, pNode3);
    ConnectListNodes(pNode3, pNode4);
    ConnectListNodes(pNode4, pNode5);

    Test("Test multinodes,delete tail node", pNode1, pNode5);
    Test("Test multinodes,delete middle node", pNode1, pNode3);
    Test("Test multinodes,delete head node", pNode1, pNode1);

    DestroyList(pNode1);
  }
} //! test
} //! Q17

namespace Q18 {
// 有序、半有序链表的查找与删除
// 题目：在一个排序的链表中，如何删除重复的结点？例如，在图3.4（a）中重复
// 结点被删除之后，链表如图3.4（b）所示。
using namespace Utilities::List;

void DeleteDuplication(ListNode **pHead) {
  if (pHead == nullptr || *pHead == nullptr)
    return;

  ListNode *pPreNode = nullptr;
  ListNode *pNode = *pHead;
  while (pNode != nullptr) {
    ListNode *pNext = pNode->m_pNext;
    bool needDelete = false;
    if (pNext != nullptr && pNext->m_nValue == pNode->m_nValue)
      needDelete = true;

    if (!needDelete) {
      pPreNode = pNode;
      pNode = pNode->m_pNext;
    } else {
      int value = pNode->m_nValue;
      ListNode *pToBeDel = pNode;
      while (pToBeDel != nullptr && pToBeDel->m_nValue == value) {
        pNext = pToBeDel->m_pNext;

        delete pToBeDel;
        pToBeDel = nullptr;

        pToBeDel = pNext;
      }

      if (pPreNode == nullptr)
        *pHead = pNext;
      else
        pPreNode->m_pNext = pNext;
      pNode = pNext;
    }
  }
}
// ====================测试代码====================
void Test(char *testName, ListNode **pHead, int *expectedValues,
          int expectedLength) {
  if (testName != nullptr)
    printf("%s begins: ", testName);

  DeleteDuplication(pHead);
  PrintList(*pHead);

  int index = 0;
  ListNode *pNode = *pHead;
  while (pNode != nullptr && index < expectedLength) {
    if (pNode->m_nValue != expectedValues[index])
      break;

    pNode = pNode->m_pNext;
    index++;
  }

  if (pNode == nullptr && index == expectedLength)
    printf("Passed.\n");
  else
    printf("FAILED.\n");
}

// 某些结点是重复的
void test() {
  printf("Start testing...\n");
  ListNode *pNode1 = CreateListNode(1);
  ListNode *pNode2 = CreateListNode(2);
  ListNode *pNode3_1 = CreateListNode(3);
  ListNode *pNode3_2 = CreateListNode(3);
  ListNode *pNode4 = CreateListNode(4);
  ListNode *pNode5_1 = CreateListNode(5);
  ListNode *pNode5_2 = CreateListNode(5);
  ListNode *pNode5_3 = CreateListNode(5);
  ListNode *pNode6 = CreateListNode(6);

  // 1,2,3,3,4,5,5,5,6
  ConnectListNodes(pNode1, pNode2);
  ConnectListNodes(pNode2, pNode3_1);
  ConnectListNodes(pNode3_1, pNode3_2);
  ConnectListNodes(pNode3_2, pNode4);
  ConnectListNodes(pNode4, pNode5_1);
  ConnectListNodes(pNode5_1, pNode5_2);
  ConnectListNodes(pNode5_2, pNode5_3);
  ConnectListNodes(pNode5_3, pNode6);

  ListNode *pHead = pNode1;

  int expectedValues[] = {1, 2, 4, 6};
  Test("Test1", &pHead, expectedValues, sizeof(expectedValues) / sizeof(int));

  DestroyList(pHead);
} //! test
} //! Q18

namespace Q19 {
// 面试题22：链表中倒数第k个结点
// 题目：输入一个链表，输出该链表中倒数第k个结点。为了符合大多数人的习惯，本题从1开始计数，
// 即链表的尾结点是倒数第1个结点。例如一个链表有6个结点，从头结点开始它们的值,
// 依次是1、2、3、4、5、6。这个链表的倒数第3个结点是值为4的结点。
// ListNode* FindKthToTail(ListNode* pListHead, unsigned int k);

// TODO:先写函数原型声明（输入参数，输出参数，返回值）和测试用例（各种边界情况）
// TODO:一定要注意用例测试，使用用例测试来辅助代码编写和边界确认等
// TODO:预处理，边界处理，循环不变量，后处理
using namespace Utilities::List;
ListNode *FindKthToTail(ListNode *pListHead, unsigned int k) {
  if (!pListHead || k == 0)
    return nullptr;
  ListNode *pcurr = pListHead;
  while (--k != 0 && pcurr->m_pNext != nullptr) {
    pcurr = pcurr->m_pNext;
  }
  ListNode *pleft = pListHead;
  if (k == 0) {
    while (pcurr->m_pNext) {
      pcurr = pcurr->m_pNext;
      pleft = pleft->m_pNext;
    }
  } else {
    pleft = nullptr;
  }

  return pleft;
}

int Test(const char *name, ListNode *pHead, unsigned int k, int expected) {
  cout << "\n==================================================\n"
       << name << ":" << k << "th from tail" << endl;
  PrintList(pHead);
  ListNode *pfound = FindKthToTail(pHead, k);
  if (pfound) {
    cout << "expected:" << expected << "\nfound:" << pfound->m_nValue << "\n"
         << boolalpha << (pfound->m_nValue == expected) << "\n\n";
  }
}

int test() {
  ListNode *pNode1 = CreateListNode(1);
  ListNode *pNode2 = CreateListNode(2);
  ListNode *pNode3 = CreateListNode(3);
  ListNode *pNode4 = CreateListNode(4);
  ListNode *pNode5 = CreateListNode(5);

  ConnectListNodes(pNode1, pNode2);
  ConnectListNodes(pNode2, pNode3);
  ConnectListNodes(pNode3, pNode4);
  ConnectListNodes(pNode4, pNode5);

  Test("Test find null", nullptr, 1, 0);
  Test("Test find tail", pNode1, 1, 5);
  Test("Test find head", pNode1, 5, 1);
  Test("Test find middle", pNode1, 3, 3);
  Test("Test find outOfRange", pNode1, 6, 0);
  Test("Test find outOfRange", pNode1, 0, 0);
  DestroyList(pNode1);

  return 0;
} //! test
} //! Q19

namespace Q20 {

// 面试题24：反转链表
// 题目：定义一个函数，输入一个链表的头结点，反转该链表并输出反转后链表的头结点。
// ListNode* ReverseList(ListNode* pHead);
using namespace Utilities::List;

ListNode *ReverseList(ListNode *pHead) {
  if (!pHead) {
    return nullptr;
  }
  if (pHead->m_pNext == nullptr)
    return pHead;

  ListNode *pcurr = pHead->m_pNext;
  ListNode *ppretail = pHead;
  ppretail->m_pNext = nullptr;
  while (pcurr->m_pNext) {
    ListNode *pnext = pcurr->m_pNext;
    pcurr->m_pNext = ppretail;
    ppretail = pcurr;
    pcurr = pnext;
  }
  pcurr->m_pNext = ppretail;
  return pcurr;
}

int Test(const char *testName, ListNode *pHead, const vector<int> &vec) {

  cout << "\n========================================\n" << testName << endl;
  PrintList(pHead);
  ListNode *pcurr = ReverseList(pHead);
  PrintList(pcurr);
  int n = 0;
  while (pcurr) {
    cout << boolalpha << (pcurr->m_nValue == vec[n]) << " ";
    ++n;
    pcurr = pcurr->m_pNext;
  }
  cout << endl;
}

int test() {
  cout << "Start testing Q20\n";
  ListNode *pNode1 = CreateListNode(1);
  ListNode *pNode2 = CreateListNode(2);
  ListNode *pNode3 = CreateListNode(3);
  ListNode *pNode4 = CreateListNode(4);
  ListNode *pNode5 = CreateListNode(5);

  ConnectListNodes(pNode1, pNode2);
  ConnectListNodes(pNode2, pNode3);
  ConnectListNodes(pNode3, pNode4);
  ConnectListNodes(pNode4, pNode5);

  Test("test null", nullptr, vector<int>(1, 1));
  Test("test singlenode", CreateListNode(1), vector<int>(1, 1));
  Test("Test multinodes", pNode1, vector<int>{5, 4, 3, 2, 1});
} //! test
} //! Q20
namespace Q21 {
// 面试题19：正则表达式匹配
// 题目：请实现一个函数用来匹配包含'.'和'*'的正则表达式。模式中的字符'.'表示任意一个字符，
// 而'*'表示它前面的字符可以出现任意次（含0次）。在本题中，匹配是指字符串的所有字符匹配整个模式。
// 例如，字符串"aaa"与模式"a.a"和"ab*ac*a"匹配，但与"aa.a"及"ab*a"均不匹配。

// TODO:有限状态机，分清楚状态，然后就是状态之间的跳转关系了
bool matchCore(const char *str, const char *pattern) {
  if (*str == '\0' && *pattern == '\0')
    return true;
  if (*str != '\0' && *pattern == '\0')
    return false;

  if (*(pattern + 1) == '*') {
    if (((*(pattern) == '.') && *str != '\0') || *pattern == *str) {
      return matchCore(str + 1, pattern + 2) || // 进入有限状态机的下一个状态
             matchCore(str + 1, pattern) || // 继续留在有限状态机的当前状态
             matchCore(str, pattern + 2); // 略过一个'*'
    } else {
      return matchCore(str, pattern + 2);
    }
  }
  if (((*(pattern) == '.') && *str != '\0') || *pattern == *str) {
    return matchCore(str + 1, pattern + 1);
  }

  return false;
}

bool match(const char *str, const char *pattern) {
  if (!str || !pattern)
    return false;
  return matchCore(str, pattern);
}

// ====================测试代码====================
void Test(const char *testName, const char *string, const char *pattern,
          bool expected) {
  cout << "\n========================================\n" << testName << endl;

  if (match(string, pattern) == expected)
    printf("Passed.\n");
  else
    printf("FAILED.\n");
}

int test() {
  Test("Test01", "", "", true);
  Test("Test02", "", ".*", true);
  Test("Test03", "", ".", false);
  Test("Test04", "", "c*", true);
  Test("Test05", "a", ".*", true);
  Test("Test06", "a", "a.", false);
  Test("Test07", "a", "", false);
  Test("Test08", "a", ".", true);
  Test("Test09", "a", "ab*", true);
  Test("Test10", "a", "ab*a", false);
  Test("Test11", "aa", "aa", true);
  Test("Test12", "aa", "a*", true);
  Test("Test13", "aa", ".*", true);
  Test("Test14", "aa", ".", false);
  Test("Test15", "ab", ".*", true); //
  Test("Test16", "ab", ".*", true);
  Test("Test17", "aaa", "aa*", true);
  Test("Test18", "aaa", "aa.a", false);
  Test("Test19", "aaa", "a.a", true);
  Test("Test20", "aaa", ".a", false);
  Test("Test21", "aaa", "a*a", true);
  Test("Test22", "aaa", "ab*a", false);
  Test("Test23", "aaa", "ab*ac*a", true);
  Test("Test24", "aaa", "ab*a*c*a", true);
  Test("Test25", "aaa", ".*", true);
  Test("Test26", "aab", "c*a*b", true);
  Test("Test27", "aaca", "ab*a*c*a", true);
  Test("Test28", "aaba", "ab*a*c*a", false);
  Test("Test29", "bbbba", ".*a*a", true);
  Test("Test30", "bcbbabab", ".*a*a", false);

  return 0;
} //! test
} //! Q21
namespace Q22 {
// 面试题20：表示数值的字符串
// 题目：请实现一个函数用来判断字符串是否表示数值（包括整数和小数）。例如，
// 字符串“+100”、“5e2”、“-123”、“3.1416”及“-1E-16”都表示数值，但“12e”、
// “1a3.14”、“1.2.3”、“+-5”及“12e+5.4”都不是

char *paraseUint(const char *digit, double &nums, int &bits) {
  if (!digit || *digit == '\0') {
    nums = NAN;
    bits = 0;
    return const_cast<char *>(digit);
  }
  nums = 0;
  bits = 0;
  while (*digit != '\0') {
    if (*digit >= '0' && *digit <= '9') {
      nums = nums * 10 + *digit - '0';
      ++bits;
      ++digit;
    } else {
      break;
    }
  }
  return const_cast<char *>(digit);
}

bool isNumeric(const char *digit, double &nums) {
  // ±123.456E±789
  // 正负号，整数部分，小数点，小数部分，指数标志，指数正负号，指数
  if (digit == nullptr || *digit == '\0') {
    nums = NAN;
    return false;
  }
  nums = 0;
  int coeff1 = 1, coeff2 = 1;
  if (*digit == '-') {
    coeff1 = -1;
    ++digit;
  } else if (*digit == '+') {
    coeff1 = 1;
    ++digit;
  }
  double base1 = 0, base2 = 0, exp = 0;
  int bits1 = 0, bits2 = 0, bits3 = 0;
  digit = paraseUint(digit, base1, bits1);
  if (*digit != '\0') {
    if (*digit == '.') {
      ++digit;
    }
    digit = paraseUint(digit, base2, bits2);
    if (*digit != '\0' && (*digit == 'E' || *digit == 'e')) {
      ++digit;
      if (*digit == '-') {
        coeff2 = -1;
        ++digit;
      } else if (*digit == '+') {
        coeff2 = +1;
        ++digit;
      }
      digit = paraseUint(digit, exp, bits3);
      if (*digit == '\0' && bits3 != 0) {
        nums = coeff1 * double(base1 + base2 * pow(0.10, bits2)) *
               pow(10, coeff2 * exp);
        return true;
      } else {
        nums = NAN;
        return false;
      }
    } else if (*digit == '\0') {
      nums = coeff1 * double(base1 + base2 * pow(0.10, bits2));
      return true;
    } else {
      nums = NAN;
      return false;
    }

  } else {
    nums = base1;
    return true;
  }
}

int Test(const char *testName, const char *str, double expected) {
  cout << "\n========================================\n" << testName << endl;
  double nums = 0;
  bool res = isNumeric(str, nums);
  cout << str << " : " << nums << endl;
  if (nums - expected < 1E-10 && nums - expected > -1E-10) {
    cout << "Passed" << endl;
  } else {
    cout << "Failed!" << endl;
  }
}

int test() {
  cout << "Test Q22" << endl;
  Test("test 0", "0", 0);
  Test("test 1e2", "1e2", 1e2);
  Test("test 123", "123", 123);
  Test("test +123.", "+123.", +123.);
  Test("test -.123", "-.123", -.123);
  Test("test -123.456", "-123.456", -123.456);
  Test("test +123.E+2", "+123.E+2", +123.E+2);
  Test("test +123.456E+2", "+123.456E+2", +123.456E+2);
  Test("test -123.456e-2", "-123.456e-2", -123.456e-2);
  Test("test -.456E-2", "-.456E-2", -.456E-2);

  Test("test 1e", "1e", 1e0);
  Test("test 1a", "1a", 1e0);
  Test("test 1.2.3", "1.2.3", 1e0);
  Test("test +-123", "+-123", 1e0);
  Test("test .", ".", 1e0);
  Test("test +.", "+.", 1e0);
  Test("test .e", ".e", 1e0);
  Test("test .e2", ".e2", 1e0);
  Test("test -..456E-2", "-..456E-2", -.456E-2);
  Test("test -.456E-2.0", "-.456E-2.0", -.456E-2);
} //! test
} //! Q22

namespace Q23 {
// 面试题21：调整数组顺序使奇数位于偶数前面
// 题目：输入一个整数数组，实现一个函数来调整该数组中数字的顺序，使得所有
// 奇数位于数组的前半部分，所有偶数位于数组的后半部分。

template <typename T> ostream &operator<<(ostream &os, const vector<T> &vec) {
  for (auto v : vec)
    os << v << " ";
  return os;
}

vector<int> reorderOddEven(vector<int> vec) {
  auto isEven = [](int x) -> bool { return x % 2 == 0; };
  auto myswap = [&](int &x, int &y) -> void {
    int t = x;
    x = y;
    y = t;
  };
  //双指针，从两端往中间移动，遇到满足条件则停止，然后交换指针。注意更新指针。
  for (int i = 0, j = vec.size() - 1; i < j; ++i, --j) {
    while (i < j && !isEven(vec[i]))
      ++i;
    while (i < j && isEven(vec[j]))
      --j;
    swap(vec[i], vec[j]);
  }
  return vec;
}

int Test(const char *testName, const vector<int> &vec) {

  cout << "\n========================================\n" << testName << endl;
  vector<int> vec2 = reorderOddEven(vec);
  cout << "original:" << vec << endl << "reordered:" << vec2 << endl;
  return 0;
}
int test() {
  Test("test1", vector<int>{8});
  Test("test2", vector<int>{1, 2, 3, 4, 5, 6, 7, 8, 9});
  Test("test3", vector<int>{9, 8, 7, 6, 5, 4, 3, 2, 1});
  return 0;
} //! test
} //! Q23

namespace Q24 {
// 面试题23：链表中环的入口结点
// 题目：一个链表中包含环，如何找出环的入口结点？
//
// 关键在于怎样获得环的长度。根据定量关系，建立数学模型，然后编程。
// 1. 使用快慢指针遍历，首次相遇时，既是环中的节点。
// 2. 使用环中节点遍历环，得到环的长度
// 3.
// 使用双指针（其中一个领先环的长度）从头开始遍历，当指针首次相遇的时候，就是环的入口节点。

using namespace Utilities::List;

ListNode *getEntryOfCircleList(ListNode *pHead) {
  if (pHead == nullptr || pHead->m_pNext == nullptr)
    return nullptr;
  ListNode *pLeft = nullptr, *pRight = nullptr;
  pLeft = pHead;
  pRight = pHead->m_pNext;

  while (pRight != pLeft) {
    if (pRight->m_pNext == nullptr || pRight->m_pNext->m_pNext == nullptr)
      return nullptr;
    pRight = pRight->m_pNext->m_pNext;
    pLeft = pLeft->m_pNext;
  }
  pRight = pRight->m_pNext;
  int cnt = 1;
  while (pRight != pLeft) {
    pRight = pRight->m_pNext;
    ++cnt;
  }

  pRight = pLeft = pHead;
  while (cnt-- > 0) {
    pRight = pRight->m_pNext;
  }
  while (pRight != pLeft) {
    pRight = pRight->m_pNext;
    pLeft = pLeft->m_pNext;
  }

  return pRight;
}

// ==================== Test Code ====================
int Test(char *testName, ListNode *pHead, ListNode *entryNode) {

  cout << "\n========================================\n" << testName << endl;

  if (getEntryOfCircleList(pHead) == entryNode)
    printf("Passed.\n");
  else
    printf("FAILED.\n");
}

int test() {
  {
    ListNode *pNode1 = CreateListNode(1);
    Test("test1", pNode1, nullptr);
  }

  {
    ListNode *pNode1 = CreateListNode(1);
    ConnectListNodes(pNode1, pNode1);

    Test("Test2", pNode1, pNode1);
  }

  {
    ListNode *pNode1 = CreateListNode(1);
    ListNode *pNode2 = CreateListNode(2);
    ListNode *pNode3 = CreateListNode(3);
    ConnectListNodes(pNode1, pNode2);
    ConnectListNodes(pNode2, pNode3);

    Test("Test3", pNode1, nullptr);
  }

  {
    ListNode *pNode1 = CreateListNode(1);
    ListNode *pNode2 = CreateListNode(2);
    ListNode *pNode3 = CreateListNode(3);
    ConnectListNodes(pNode3, pNode2);
    ConnectListNodes(pNode1, pNode2);
    ConnectListNodes(pNode2, pNode3);

    Test("Test4", pNode1, pNode2);
  }
} //! test
} //! Q24

namespace Q25 {
// 面试题52：两个链表的第一个公共结点
// 题目：输入两个链表，找出它们的第一个公共结点。
using namespace Utilities::List;
ListNode *findFirstCommonNode(ListNode *plhead, ListNode *prhead) {
  if (plhead == nullptr || prhead == nullptr) {
    return nullptr;
  }
  int lenl = 0, lenr = 0;
  ListNode *plnode = plhead, *prnode = prhead;

  while (plnode) {
    ++lenl;
    plnode = plnode->m_pNext;
  }
  while (prnode) {
    ++lenr;
    prnode = prnode->m_pNext;
  }
  ListNode *plong = nullptr;
  ListNode *psmall = nullptr;
  int len = 0;

  // 分支预测，覆盖所有情况
  if (lenl >= lenr) {
    len = lenl - lenr;
    plong = plhead;
    psmall = prhead;
  } else {
    len = lenr - lenl;
    plong = prhead;
    psmall = plhead;
  }

  while (len > 0) {
    plong = plong->m_pNext;
    --len;
  }
  while (plong != psmall) {
    plong = plong->m_pNext;
    psmall = psmall->m_pNext;
  }
  return plong;
}

int Test(const char *testName, ListNode *plhead, ListNode *prhead,
         ListNode *expected) {

  cout << "\n========================================\n" << testName << endl;
  cout << "First List   :";
  PrintList(plhead);
  cout << "Second List  :";
  PrintList(prhead);
  ListNode *pfound = findFirstCommonNode(plhead, prhead);
  cout << "Expected List:";
  PrintList(expected);
  cout << "Found List   :";
  PrintList(pfound);
  if (pfound == expected) {
    cout << "Passed." << endl;
  } else {
    cout << "Failed." << endl;
  }
  return 0;
}
void DestroyNode(ListNode *pNode) {
  delete pNode;
  pNode = nullptr;
}
int test() {
  { Test("Test1", nullptr, nullptr, nullptr); }

  {
    // 没有公共结点
    // 1 - 2 - 3 - 4
    //
    // 5 - 6 - 7
    ListNode *pNode1 = CreateListNode(1);
    ListNode *pNode2 = CreateListNode(2);
    ListNode *pNode3 = CreateListNode(3);
    ListNode *pNode4 = CreateListNode(4);
    ListNode *pNode5 = CreateListNode(5);
    ListNode *pNode6 = CreateListNode(6);
    ListNode *pNode7 = CreateListNode(7);

    ConnectListNodes(pNode1, pNode2);
    ConnectListNodes(pNode2, pNode3);
    ConnectListNodes(pNode3, pNode4);
    ConnectListNodes(pNode5, pNode6);
    ConnectListNodes(pNode6, pNode7);

    Test("Test2", pNode1, pNode5, nullptr);

    DestroyList(pNode1);
    DestroyList(pNode5);
  }
  {

    // 第一个公共结点在链表中间
    // 1 - 2 - 3 \
    //            6 - 7
    //     4 - 5 /

    ListNode *pNode1 = CreateListNode(1);
    ListNode *pNode2 = CreateListNode(2);
    ListNode *pNode3 = CreateListNode(3);
    ListNode *pNode4 = CreateListNode(4);
    ListNode *pNode5 = CreateListNode(5);
    ListNode *pNode6 = CreateListNode(6);
    ListNode *pNode7 = CreateListNode(7);

    ConnectListNodes(pNode1, pNode2);
    ConnectListNodes(pNode2, pNode3);
    ConnectListNodes(pNode3, pNode6);
    ConnectListNodes(pNode4, pNode5);
    ConnectListNodes(pNode5, pNode6);
    ConnectListNodes(pNode6, pNode7);

    Test("Test3", pNode1, pNode4, pNode6);

    ConnectListNodes(pNode5, nullptr);
    DestroyList(pNode1);
    DestroyList(pNode4);
  }

  {
    // 公共结点是第一个结点
    // 1 - 2 - 3 - 4 - 5
    // 两个链表完全重合
    ListNode *pNode1 = CreateListNode(1);
    ListNode *pNode2 = CreateListNode(2);
    ListNode *pNode3 = CreateListNode(3);
    ListNode *pNode4 = CreateListNode(4);
    ListNode *pNode5 = CreateListNode(5);

    ConnectListNodes(pNode1, pNode2);
    ConnectListNodes(pNode2, pNode3);
    ConnectListNodes(pNode3, pNode4);
    ConnectListNodes(pNode4, pNode5);

    Test("Test4", pNode1, pNode1, pNode1);

    DestroyList(pNode1);
  }
} //! test
} //! Q25

namespace Q26 {
// 面试题25：合并两个排序的链表
// 题目：输入两个递增排序的链表，合并这两个链表并使新链表中的结点仍然是按
// 照递增排序的。例如输入图3.11中的链表1和链表2，则合并之后的升序链表如链
using namespace Utilities::List;
ListNode *mergeSortedLists(ListNode *pList1, ListNode *pList2) {
  ListNode *pMerged;
  if (pList1 == nullptr) {
    pMerged = pList2;
    return pMerged;
  }
  if (pList2 == nullptr) {
    pMerged = pList1;
    return pMerged;
  }
  if (pList1->m_nValue < pList2->m_nValue) {
    pMerged = pList1;
    pList1 = pList1->m_pNext;
  } else {
    pMerged = pList2;
    pList2 = pList2->m_pNext;
  }
  ListNode *pCurr = pMerged;
  while (pList1 && pList2) {
    if (pList1->m_nValue < pList2->m_nValue) {
      pCurr->m_pNext = pList1;
      pList1 = pList1->m_pNext;
    } else {
      pCurr->m_pNext = pList2;
      pList2 = pList2->m_pNext;
    }
    pCurr = pCurr->m_pNext;
  }
  pCurr->m_pNext = pList1 ? pList1 : pList2;
  return pMerged;
}
// ====================测试代码====================
ListNode *Test(char *testName, ListNode *pHead1, ListNode *pHead2) {

  cout << "\n========================================\n" << testName << endl;

  printf("The first list is:\n");
  PrintList(pHead1);

  printf("The second list is:\n");
  PrintList(pHead2);

  printf("The merged list is:\n");
  ListNode *pMergedHead = mergeSortedLists(pHead1, pHead2);
  PrintList(pMergedHead);

  printf("\n\n");

  return pMergedHead;
}

void test() {
  {
    // 两个链表都为空链表
    // list1: 空链表
    // list2: 空链表
    ListNode *pMergedHead = Test("Test1", nullptr, nullptr);
  }
  {
    // 一个链表为空链表
    // list1: 1->3->5
    // list2: 空链表

    ListNode *pNode1 = CreateListNode(1);
    ListNode *pNode3 = CreateListNode(3);
    ListNode *pNode5 = CreateListNode(5);
    ConnectListNodes(pNode1, pNode3);
    ConnectListNodes(pNode3, pNode5);

    ListNode *pMergedHead = Test("Test2", pNode1, nullptr);

    DestroyList(pMergedHead);
  }
  {
    // 两个链表都只有一个数字
    // list1: 1
    // list2: 2
    ListNode *pNode1 = CreateListNode(1);
    ListNode *pNode2 = CreateListNode(2);
    ListNode *pMergedHead = Test("Test3", pNode1, pNode2);
    DestroyList(pMergedHead);
  }
  {
    // 两个链表中有重复的数字
    // list1: 1->3->5
    // list2: 1->3->5

    ListNode *pNode1 = CreateListNode(1);
    ListNode *pNode3 = CreateListNode(3);
    ListNode *pNode5 = CreateListNode(5);
    ConnectListNodes(pNode1, pNode3);
    ConnectListNodes(pNode3, pNode5);

    ListNode *pNode2 = CreateListNode(1);
    ListNode *pNode4 = CreateListNode(3);
    ListNode *pNode6 = CreateListNode(5);
    ConnectListNodes(pNode2, pNode4);
    ConnectListNodes(pNode4, pNode6);

    ListNode *pMergedHead = Test("Test4", pNode1, pNode2);
    DestroyList(pMergedHead);
  }

  {
    // list1: 1->3->5
    // list2: 2->4->6
    ListNode *pNode1 = CreateListNode(1);
    ListNode *pNode3 = CreateListNode(3);
    ListNode *pNode5 = CreateListNode(5);

    ConnectListNodes(pNode1, pNode3);
    ConnectListNodes(pNode3, pNode5);

    ListNode *pNode2 = CreateListNode(2);
    ListNode *pNode4 = CreateListNode(4);
    ListNode *pNode6 = CreateListNode(6);

    ConnectListNodes(pNode2, pNode4);
    ConnectListNodes(pNode4, pNode6);

    ListNode *pMergedHead = Test("Test5", pNode1, pNode2);

    DestroyList(pMergedHead);
  }
} //! test
} //! Q26

namespace Q27 {

// 面试题30：包含min函数的栈
// 题目：定义栈的数据结构，请在该类型中实现一个能够得到栈的最小元素的min函数。
// 要求:在该栈中，调用min、push、pop、top的时间复杂度都是O(1)。
// 提示：使用两个数据核心，以空间换取时间。一个用来保存真正的数据，另一个用来用来保存最小值。
// TODO:声明与定义是不同的!
// template <>class CMinStack<int>{};
template <typename T> class CMinStack {
public:
  //  CMinStack()=delete;
  //  CMinStack &operator=(CMinStack &) = delete;
  //  CMinStack(CMinStack &) = delete;
  const T &min();
  //  const T &min() {
  //    int len = checkSize(1);
  //    return m_mindata[len - 1];
  //  }
  void push(const T &item) {
    int len = checkSize(0);
    m_data.push_back(item);
    if (len != 0) {
      m_mindata.push_back(item < m_mindata[len - 1] ? item
                                                    : m_mindata[len - 1]);
    } else {
      m_mindata.push_back(item);
    }
  }
  void pop() {
    checkSize(1);
    m_data.pop_back();
    m_mindata.pop_back();
  }
  const T &top() {
    int len = checkSize(1);
    return m_data[len - 1];
  }
  inline size_t size() const { return m_data.size(); }
  inline bool empty() const { return m_data.size() == 0; }

  friend ostream &operator<<(ostream &os, const CMinStack<T> &stack);

private:
  inline size_t checkSize(int len) {
    assert(m_data.size() >= len);
    return m_data.size();
  }

  vector<T> m_data;
  vector<T> m_mindata;

}; //! CMinStack
template <typename T> const T &CMinStack<T>::min() {
  int len = checkSize(1);
  return m_mindata[len - 1];
}
template <typename T>
ostream &operator<<(ostream &os, const CMinStack<T> &stack) {
  for (const T &item : stack.m_data) {
    os << item << " ";
  }
  return os;
}

void Test(const char *testName, CMinStack<int> &stack, int expected) {

  cout << "\n========================================\n" << testName << endl;

  if (stack.min() == expected)
    printf("Passed.\n");
  else
    printf("Failed.\n");
}

int test() {
  CMinStack<int> stack;
  stack.push(3);
  Test("Test1", stack, 3);

  stack.push(4);
  Test("Test2", stack, 3);

  stack.push(2);
  Test("Test3", stack, 2);

  stack.push(3);
  Test("Test4", stack, 2);

  stack.pop();
  Test("Test5", stack, 2);

  stack.pop();
  Test("Test6", stack, 3);

  stack.pop();
  Test("Test7", stack, 3);

  stack.push(0);
  Test("Test8", stack, 0);

  return 0;
} //! test

} //! Q27

namespace Q28 {
// 面试题59（一）：滑动窗口的最大值
// 题目：给定一个数组和滑动窗口的大小，请找出所有滑动窗口里的最大值。例如，
// 如果输入数组{2, 3, 4, 2, 6, 2, 5, 1}及滑动窗口的大小3，那么一共存在6个
// 滑动窗口，它们的最大值分别为{4, 4, 6, 6, 6, 5}，
// 使用双端队列，尾部插入，头部删除，保存当前滑动窗中的内容。
//同时使用index保存最大值的序号。插入删除的时候比较最大值与当前值的关系。

vector<int> getMaxOfSliddingWindowsOfVectors(vector<int> &vec, int len) {
  vector<int> maxvec;
  int sz = vec.size();
  if (sz >= len) {
    int maxval = vec[0], maxidx = 0;
    for (int i = 0; i < len; ++i) {
      if (maxval <= vec[i]) {
        maxval = vec[i];
        maxidx = i;
      }
    }
    maxvec.push_back(maxval);

    for (int i = len; i < sz; ++i) {
      if (vec[i] >= maxval) {
        maxval = vec[i];
        maxidx = i;
      } else if (maxidx + len >= i) {
        maxval = vec[i];
        maxidx = i;
        for (int k = i - len + 1; k <= i; ++k) {
          if (maxval <= vec[k]) {
            maxval = vec[k];
            maxidx = k;
          }
        }
      }
      maxvec.push_back(maxval);
    }
  }
  return maxvec;
}

int Test(const char *testName, vector<int> &vec, int n, vector<int> &expected) {
  cout << "\n========================================\n" << testName << endl;
  vector<int> res = getMaxOfSliddingWindowsOfVectors(vec, n);
  cout << "Original vector:" << vec << endl;
  cout << "Expected vector:" << expected << endl;
  cout << "Result vector  :" << res << endl;
  cout << "Passed ? : " << boolalpha << myequal(res, expected) << endl;
  return 0;
}

int test() {
  vector<int> vec = {2, 3, 4, 2, 6, 2, 5, 1};
  vector<int> expected_1 = {2, 3, 4, 2, 6, 2, 5, 1};
  vector<int> expected_2 = {3, 4, 4, 6, 6, 5, 5};
  vector<int> expected_3 = {4, 4, 6, 6, 6, 5};
  vector<int> expected_4 = {4, 6, 6, 6, 6};
  vector<int> expected_8 = {6};
  vector<int> expected_9 = {};

  Test("test1", vec, 1, expected_1);
  Test("test2", vec, 2, expected_2);
  Test("test3", vec, 3, expected_3);
  Test("test4", vec, 4, expected_4);
  Test("test8", vec, 8, expected_8);
  Test("test9", vec, 9, expected_9);
  return 0;
}
} //! Q28

namespace Q29 {
// 面试题63：股票的最大利润
// 题目：假设把某股票的价格按照时间先后顺序存储在数组中，请问买卖交易该股票可能获得的利润是多少？
// 例如一只股票在某些时间节点的价格为{9, 11, 8, 5,7, 12, 16, 14}。
// 如果我们能在价格为5的时候买入并在价格为16时卖出，则能收获最大的利润11。

// 目标：最大利润，也就是相对差值。
// 转化：可以先相邻做差，得到差值序列，然后使用自底向下的动态规划，求差值序列的连续子数组最大和。
// 动态规划分为两种，一种是自带备忘录的自顶向下的方法，一种是自底向上的方法。

//临时子数组最大值maxsumtmp，与最大值maxsumval
//子数组区间，动态调整左右端点：lhs，rhs

vector<int> maxSumOfSeqSubArray(vector<int> vec) {
  int sz = 0;
  if ((sz = vec.size()) == 0) {
    return vector<int>();
  }
  int lhs = 0, rhs = 0;
  int maxsumval = vec[0];
  int maxsumtmp = vec[0];
  for (int i = 0; i < sz; ++i) {
    if (maxsumtmp <= 0 && maxsumtmp > vec[i])
      continue;
    else if (maxsumtmp <= 0 && maxsumtmp <= vec[i]) {
      maxsumtmp = vec[i];
      lhs = i;
      rhs = i;
    } else {
      maxsumtmp += vec[i];
      if (maxsumtmp > maxsumval) {
        rhs = i;
        maxsumval = maxsumtmp;
      }
    }
  }
  cout << "lhs:" << lhs << ", rhs:" << rhs << endl;
  return vector<int>(vec.begin() + lhs, vec.begin() + rhs + 1);
}

int Test(const char *testName, vector<int> &vec, vector<int> &expected) {
  cout << "\n========================================\n" << testName << endl;
  vector<int> res = maxSumOfSeqSubArray(vec);
  cout << "Original vector:" << vec << endl;
  cout << "Expected vector:" << expected << endl;
  cout << "Result vector  :" << res << endl;
  cout << "Passed ? : " << boolalpha << myequal(res, expected) << endl;
  return 0;
}

int test() {
  {
    vector<int> vec = {2, -3, -3, 2, 5, 4, -2};
    vector<int> expected = {2, 5, 4};
    Test("test1", vec, expected);
  }

  {
    vector<int> vec = {2, -3, -3, 2, -5, 4, -2};
    vector<int> expected = {4};
    Test("test2", vec, expected);
  }

  {
    vector<int> vec = {-2, -3, -3, -1, -5, -4, -2};
    vector<int> expected = {-1};
    Test("test3", vec, expected);
  }

  return 0;
}
} //! Q29

namespace Q30 {
int FindNumberAppearingOnce(int numbers[], int length) {
  if (numbers == nullptr || length <= 0)
    throw new std::runtime_error(std::string("Invalid input."));

  int bitSum[32] = {0};
  for (int i = 0; i < length; ++i) {
    int bitMask = 1;
    for (int j = 31; j >= 0; --j) {
      int bit = numbers[i] & bitMask;
      if (bit != 0)
        bitSum[j] += 1;

      bitMask = bitMask << 1;
    }
  }

  int result = 0;
  for (int i = 0; i < 32; ++i) {
    result = result << 1;
    result += bitSum[i] % 3;
  }

  return result;
}

// ====================测试代码====================
void Test(const char *testName, int numbers[], int length, int expected) {
  int result = FindNumberAppearingOnce(numbers, length);
  if (result == expected)
    printf("%s passed.\n", testName);
  else
    printf("%s FAILED.\n", testName);
}
int test() {
  {
    int numbers[] = {1, 1, 2, 2, 2, 1, 3};
    int expected = 3;
    Test("Test1", numbers, sizeof(numbers) / sizeof(int), expected);
  }
  {
    int numbers[] = {-10, 214, 214, 214};
    int expected = -10;
    Test("Test2", numbers, sizeof(numbers) / sizeof(int), expected);
  }
  {
    int numbers[] = {-23, 0, 214, -23, 214, -23, 214};
    int expected = 0;
    Test("Test3", numbers, sizeof(numbers) / sizeof(int), expected);
  }
  {
    int numbers[] = {0, 3467, 0, 0, 0, 0, 0, 0};
    int expected = 3467;
    Test("Test4", numbers, sizeof(numbers) / sizeof(int), expected);
  }
} //! test
} //! Q30

namespace Q31 {

// 使用栈与队列辅助树的遍历
// 数据结构-前序遍历、中序遍历、后序遍历、层级遍历(递归、非递归)
// 原文 http://blog.csdn.net/juanqinyang/article/details/51395927

struct BiTreeNode {
  BiTreeNode(int data = 0)
      : m_pLeft(nullptr), m_pRight(nullptr), m_nVal(data) {}
  BiTreeNode(BiTreeNode &) = delete;
  BiTreeNode &operator=(BiTreeNode &) = delete;

  BiTreeNode *m_pLeft;
  BiTreeNode *m_pRight;
  int m_nVal;
};

BiTreeNode *CreateBiTreeNode(int data) { return new BiTreeNode(data); }

void ConnectBiTreeNode(BiTreeNode *parent, BiTreeNode *plBiTreeNode,
                       BiTreeNode *prBiTreeNode) {
  assert(parent != nullptr);
  parent->m_pLeft = plBiTreeNode;
  parent->m_pRight = prBiTreeNode;
}

void DestroyBiTree(BiTreeNode *root) {
  if (root == nullptr)
    return;
  BiTreeNode *plchild = root->m_pLeft;
  BiTreeNode *prchild = root->m_pRight;
  delete root;

  DestroyBiTree(plchild);
  DestroyBiTree(prchild);
}

auto visitBiTreeNode = [](BiTreeNode *root) {
  cout << setw(3) << root->m_nVal;
};
enum class Order { preOrder, inOrder, postOrder };

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
    // cout << setw(3) << "#";
  }
}

void preOrderTraverseNonRec(BiTreeNode *root) {
  if (root == nullptr) {
    cout << "Empty tree!\n";
    return;
  }

#ifdef _DEBUG
  {
    // Method 1
    stack<BiTreeNode *> s;
    BiTreeNode *pcurr = root;

    while (pcurr != nullptr || !s.empty()) {
      while (pcurr != nullptr) {
        visitBiTreeNode(pcurr);
        s.push(pcurr);
        pcurr = pcurr->m_pLeft;
      }
      if (!s.empty()) {
        pcurr = s.top();
        s.pop();
        pcurr = pcurr->m_pRight;
      }
    }
  }
#else  //_DEBUG
  {
    // Method 2
    stack<BiTreeNode *> s;
    BiTreeNode *pcurr = nullptr;
    s.push(root);

    while (!s.empty()) {
      pcurr = s.top();
      visitBiTreeNode(pcurr);
      s.pop();

      if (pcurr->m_pRight)
        s.push(pcurr->m_pRight);
      if (pcurr->m_pLeft)
        s.push(pcurr->m_pLeft);
    }
  }
#endif //!_DEBUG
}
void inOrderTraverseNonRec(BiTreeNode *root) {
  if (root == nullptr) {
    cout << "Empty tree!\n";
    return;
  }

  // Method 1
  stack<BiTreeNode *> s;
  BiTreeNode *pcurr = root;

  while (pcurr != nullptr || !s.empty()) {
    while (pcurr != nullptr) {
      s.push(pcurr);
      pcurr = pcurr->m_pLeft;
    }
    if (!s.empty()) {
      pcurr = s.top();
      visitBiTreeNode(pcurr);
      s.pop();
      pcurr = pcurr->m_pRight;
    }
  }
}
void postOrderTraverseNonRec(BiTreeNode *root) {
  if (root == nullptr) {
    cout << "Empty tree!\n";
    return;
  }

  // Method 1
  stack<BiTreeNode *> s;
  BiTreeNode *pcurr = root, *pre = nullptr;

  while (pcurr != nullptr || !s.empty()) {
    while (pcurr != nullptr) {
      s.push(pcurr);
      pcurr = pcurr->m_pLeft;
    }
    if (!s.empty()) {
      pcurr = s.top();
      if (pcurr->m_pRight == nullptr || pcurr->m_pRight == pre) {
        visitBiTreeNode(pcurr);
        pre = pcurr;
        s.pop();
        pcurr = nullptr;
      } else {
        pcurr = pcurr->m_pRight;
      }
    }
  }
}

int getBiTreeHeight(BiTreeNode *root) {
  return (root == nullptr) ? 0 : max(getBiTreeHeight(root->m_pLeft) + 1,
                                     getBiTreeHeight(root->m_pRight) + 1);
}

void printBiTreeLevel(BiTreeNode *root, int level) {
  if (root == nullptr)
    return;
  if (level == 1) {
    visitBiTreeNode(root);
    return;
  }
  printBiTreeLevel(root->m_pLeft, level - 1);
  printBiTreeLevel(root->m_pRight, level - 1);
  return;
}

void levelTraverseRec(BiTreeNode *root) {

  int height = getBiTreeHeight(root);
  for (int i = 1; i <= height; ++i) {
    printBiTreeLevel(root, i);
  }
}

void levelTraverseNonRec(BiTreeNode *root) {
  if (root == NULL)
    return;

  BiTreeNode *pcurr = root;
  queue<BiTreeNode *> q;
  q.push(root);

  while (!q.empty()) {
    pcurr = q.front();
    visitBiTreeNode(pcurr);
    q.pop();

    if (pcurr->m_pLeft)
      q.push(pcurr->m_pLeft);
    if (pcurr->m_pRight)
      q.push(pcurr->m_pRight);
  }
}
int Test(const char *testName, BiTreeNode *root) {
  cout << "\n========================================\n" << testName << endl;
  cout << "\nPreOrderTraverseRec:" << endl;
  traverseBiTreeRec(root, Order::preOrder);
  cout << "\nInOrderTraverseRec:" << endl;
  traverseBiTreeRec(root, Order::inOrder);
  cout << "\nPostOrderTraverseRec:" << endl;
  traverseBiTreeRec(root, Order::postOrder);
  cout << "\nLevelTraverseRec:" << endl;
  levelTraverseRec(root);

  cout << "\nPreOrderTraverseNonRec:" << endl;
  preOrderTraverseNonRec(root);
  cout << "\nInOrderTraverseNonRec:" << endl;
  inOrderTraverseNonRec(root);
  cout << "\nPostOrderTraverseNonRec:" << endl;
  postOrderTraverseNonRec(root);
  cout << "\nLevelTraverseNonRec:" << endl;
  levelTraverseNonRec(root);
  cout << endl;
}

int test() {
  cout << "Starting test Q31" << endl;
  BiTreeNode *pBiTreeNode1 = CreateBiTreeNode(1);
  BiTreeNode *pBiTreeNode2 = CreateBiTreeNode(2);
  BiTreeNode *pBiTreeNode3 = CreateBiTreeNode(3);
  BiTreeNode *pBiTreeNode21 = CreateBiTreeNode(21);
  BiTreeNode *pBiTreeNode22 = CreateBiTreeNode(22);
  BiTreeNode *pBiTreeNode31 = CreateBiTreeNode(31);
  BiTreeNode *pBiTreeNode32 = CreateBiTreeNode(32);

  ConnectBiTreeNode(pBiTreeNode1, pBiTreeNode2, pBiTreeNode3);
  ConnectBiTreeNode(pBiTreeNode2, pBiTreeNode21, pBiTreeNode22);
  ConnectBiTreeNode(pBiTreeNode3, pBiTreeNode31, pBiTreeNode32);

  Test("test1", pBiTreeNode1);

  return 0;
} //! test
} //! Q31
namespace Q32 {
using namespace Utilities::BiTree;
// 面试题26：树的子结构
// 题目：输入两棵二叉树A和B，判断B是不是A的子结构。
// TODO:边界测试的时候，注意规定什么是正确的，什么不是正确的
bool myeauqls(double lhs, double rhs) {
  return (lhs - rhs) > -1E-10 && (lhs - rhs) < 1E-10;
}

bool hasSubTreeCore(BiTreeNode *proot1, BiTreeNode *proot2) {
  if (proot2 == nullptr) {
    return true;
  }
  if (proot1 == nullptr && proot2 != nullptr) {
    return false;
  }
  return myeauqls(proot1->m_nValue, proot2->m_nValue) &&
         hasSubTreeCore(proot1->m_pLeft, proot2->m_pLeft) &&
         hasSubTreeCore(proot1->m_pRight, proot2->m_pRight);
}

bool hasSubTree(BiTreeNode *proot1, BiTreeNode *proot2) {
  if (proot2 == nullptr) {
    return true;
  }
  if (proot1 == nullptr && proot2 != nullptr) {
    return false;
  }
  bool res = hasSubTreeCore(proot1, proot2);
  if (!res)
    res = hasSubTree(proot1->m_pLeft, proot2);
  if (!res)
    res = hasSubTree(proot1->m_pRight, proot2);
  return res;
}

// ====================测试代码====================
void Test(char *testName, BiTreeNode *pRoot1, BiTreeNode *pRoot2,
          bool expected) {

  cout << "\n========================================\n" << testName << endl;
  if (hasSubTree(pRoot1, pRoot2) == expected)
    printf("%s passed.\n", testName);
  else
    printf("%s failed.\n", testName);
}
int test() {
  { Test("Test1", nullptr, nullptr, true); }
  {
    BiTreeNode *pBiTreeNodeA1 = CreateBiTreeNode(8);
    BiTreeNode *pBiTreeNodeA2 = CreateBiTreeNode(9);
    BiTreeNode *pBiTreeNodeA3 = CreateBiTreeNode(3);
    BiTreeNode *pBiTreeNodeA4 = CreateBiTreeNode(2);

    ConnectBiTreeNodes(pBiTreeNodeA1, nullptr, pBiTreeNodeA2);
    ConnectBiTreeNodes(pBiTreeNodeA2, pBiTreeNodeA3, pBiTreeNodeA4);

    Test("Test2", pBiTreeNodeA1, nullptr, true);
    Test("Test3", nullptr, pBiTreeNodeA1, false);

    DestroyTree(pBiTreeNodeA1);
  }

  {
    // 树中结点含有分叉，树B是树A的子结构
    //                  8                8
    //              /       \           / \
    //             8         7         9   2
    //           /   \
    //          9     2
    //               / \
    //              4   7
    BiTreeNode *pBiTreeNodeA1 = CreateBiTreeNode(8);
    BiTreeNode *pBiTreeNodeA2 = CreateBiTreeNode(8);
    BiTreeNode *pBiTreeNodeA3 = CreateBiTreeNode(7);
    BiTreeNode *pBiTreeNodeA4 = CreateBiTreeNode(9);
    BiTreeNode *pBiTreeNodeA5 = CreateBiTreeNode(2);
    BiTreeNode *pBiTreeNodeA6 = CreateBiTreeNode(4);
    BiTreeNode *pBiTreeNodeA7 = CreateBiTreeNode(7);

    ConnectBiTreeNodes(pBiTreeNodeA1, pBiTreeNodeA2, pBiTreeNodeA3);
    ConnectBiTreeNodes(pBiTreeNodeA2, pBiTreeNodeA4, pBiTreeNodeA5);
    ConnectBiTreeNodes(pBiTreeNodeA5, pBiTreeNodeA6, pBiTreeNodeA7);

    BiTreeNode *pBiTreeNodeB1 = CreateBiTreeNode(8);
    BiTreeNode *pBiTreeNodeB2 = CreateBiTreeNode(9);
    BiTreeNode *pBiTreeNodeB3 = CreateBiTreeNode(2);

    ConnectBiTreeNodes(pBiTreeNodeB1, pBiTreeNodeB2, pBiTreeNodeB3);

    Test("Test4", pBiTreeNodeA1, pBiTreeNodeB1, true);

    DestroyTree(pBiTreeNodeA1);
    DestroyTree(pBiTreeNodeB1);
  }

  {
    // 树中结点含有分叉，树B不是树A的子结构
    //                  8                8
    //              /       \           / \
    //             8         7         9   2
    //           /   \
    //          9     3
    //               / \
    //              4   7
    BiTreeNode *pBiTreeNodeA1 = CreateBiTreeNode(8);
    BiTreeNode *pBiTreeNodeA2 = CreateBiTreeNode(8);
    BiTreeNode *pBiTreeNodeA3 = CreateBiTreeNode(7);
    BiTreeNode *pBiTreeNodeA4 = CreateBiTreeNode(9);
    BiTreeNode *pBiTreeNodeA5 = CreateBiTreeNode(3);
    BiTreeNode *pBiTreeNodeA6 = CreateBiTreeNode(4);
    BiTreeNode *pBiTreeNodeA7 = CreateBiTreeNode(7);

    ConnectBiTreeNodes(pBiTreeNodeA1, pBiTreeNodeA2, pBiTreeNodeA3);
    ConnectBiTreeNodes(pBiTreeNodeA2, pBiTreeNodeA4, pBiTreeNodeA5);
    ConnectBiTreeNodes(pBiTreeNodeA5, pBiTreeNodeA6, pBiTreeNodeA7);

    BiTreeNode *pBiTreeNodeB1 = CreateBiTreeNode(8);
    BiTreeNode *pBiTreeNodeB2 = CreateBiTreeNode(9);
    BiTreeNode *pBiTreeNodeB3 = CreateBiTreeNode(2);

    ConnectBiTreeNodes(pBiTreeNodeB1, pBiTreeNodeB2, pBiTreeNodeB3);

    Test("Test5", pBiTreeNodeA1, pBiTreeNodeB1, false);

    DestroyTree(pBiTreeNodeA1);
    DestroyTree(pBiTreeNodeB1);
  }
} //! test
} //! Q32

namespace Q33 {
// MirrorOfBiTree
using namespace Utilities::BiTree;
void mirrorBiTree(BiTreeNode *proot) {
  if (proot == nullptr) {
    return;
  }
  BiTreeNode *pBiTreeNode = proot->m_pLeft;
  proot->m_pLeft = proot->m_pRight;
  proot->m_pRight = pBiTreeNode;
  mirrorBiTree(proot->m_pLeft);
  mirrorBiTree(proot->m_pRight);
}

int Test(const char *testName, BiTreeNode *proot) {
  cout << "\n========================================\n" << testName << endl;
  cout << "PreMirror preOrder  :";
  traverseBiTreeRec(proot, Order::preOrder);
  cout << endl;
  mirrorBiTree(proot);
  cout << "PostMirror preOrder :";
  traverseBiTreeRec(proot, Order::preOrder);
  cout << endl;
}

int test() {
  {
    // 树中结点含有分叉，树B是树A的子结构
    //                  8                8
    //              /       \           / \
    //             8         7         9   2
    //           /   \
    //          9     2
    //               / \
    //              4   7
    BiTreeNode *pBiTreeNodeA1 = CreateBiTreeNode(8);
    BiTreeNode *pBiTreeNodeA2 = CreateBiTreeNode(8);
    BiTreeNode *pBiTreeNodeA3 = CreateBiTreeNode(7);
    BiTreeNode *pBiTreeNodeA4 = CreateBiTreeNode(9);
    BiTreeNode *pBiTreeNodeA5 = CreateBiTreeNode(2);
    BiTreeNode *pBiTreeNodeA6 = CreateBiTreeNode(4);
    BiTreeNode *pBiTreeNodeA7 = CreateBiTreeNode(7);

    ConnectBiTreeNodes(pBiTreeNodeA1, pBiTreeNodeA2, pBiTreeNodeA3);
    ConnectBiTreeNodes(pBiTreeNodeA2, pBiTreeNodeA4, pBiTreeNodeA5);
    ConnectBiTreeNodes(pBiTreeNodeA5, pBiTreeNodeA6, pBiTreeNodeA7);

    BiTreeNode *pBiTreeNodeB1 = CreateBiTreeNode(8);
    BiTreeNode *pBiTreeNodeB2 = CreateBiTreeNode(9);
    BiTreeNode *pBiTreeNodeB3 = CreateBiTreeNode(2);

    ConnectBiTreeNodes(pBiTreeNodeB1, pBiTreeNodeB2, pBiTreeNodeB3);

    Test("Test1", pBiTreeNodeA1);
    Test("Test2", pBiTreeNodeB1);

    DestroyTree(pBiTreeNodeA1);
    DestroyTree(pBiTreeNodeB1);
  }
} //! test
} //! Q33

namespace Q34 {
// 面试题28：对称的二叉树
// 题目：请实现一个函数，用来判断一棵二叉树是不是对称的。
// 其中：如果一棵二叉树和它的镜像一样，那么它是对称的。

// 有时候，一般性问题比特殊问题更容易解决。
// 因为特殊问题把条件给限定了，容易限制人的思维。
// 而一般问题，也就是抽象问题，让问题扩大化，忽略细节，从而抓住主要的地方。
using namespace Utilities::BiTree;
auto myequal = [](double a, double b) -> bool {
  return (a - b <= 1E-10 && b - a <= 1E-10);
};
bool isSymmetrical(BiTreeNode *proot1, BiTreeNode *proot2) {
  if (proot1 == nullptr && proot2 == nullptr) {
    return true;
  }
  if (proot1 == nullptr || proot2 == nullptr) {
    return false;
  }
  return myequal(proot1->m_nValue, proot2->m_nValue) &&
         isSymmetrical(proot1->m_pLeft, proot2->m_pRight) &&
         isSymmetrical(proot1->m_pRight, proot2->m_pLeft);
}
bool isSymmetrical(BiTreeNode *proot) { return isSymmetrical(proot, proot); }

void Test(char *testName, BiTreeNode *pRoot, bool expected) {
  cout << "\n========================================\n" << testName << endl;

  if (isSymmetrical(pRoot) == expected)
    cout << "Passed.\n";
  else
    cout << "FAILED.\n";
}
void test() {
  //            8
  //        6      6
  //       5 7    7 5
  BiTreeNode *pNode8 = CreateBiTreeNode(8);
  BiTreeNode *pNode61 = CreateBiTreeNode(6);
  BiTreeNode *pNode62 = CreateBiTreeNode(6);
  BiTreeNode *pNode51 = CreateBiTreeNode(5);
  BiTreeNode *pNode71 = CreateBiTreeNode(7);
  BiTreeNode *pNode72 = CreateBiTreeNode(7);
  BiTreeNode *pNode52 = CreateBiTreeNode(5);

  ConnectBiTreeNodes(pNode8, pNode61, pNode62);
  ConnectBiTreeNodes(pNode61, pNode51, pNode71);
  ConnectBiTreeNodes(pNode62, pNode72, pNode52);

  Test("Test1", pNode8, true);
  SetBiTreeNodeValue(pNode52, 0);
  Test("Test1", pNode8, false);

  DestroyTree(pNode8);
} //! test

} //! Q34

namespace Q35 {

// 面试题54：二叉搜索树的第k个结点
// 题目：给定一棵二叉搜索树，请找出其中的第k大的结点。
using namespace Utilities::BiTree;
const BiTreeNode *KthNodeCore(const BiTreeNode *pRoot, unsigned int &k) {
  const BiTreeNode *target = nullptr;

  if (pRoot->m_pLeft != nullptr)
    target = KthNodeCore(pRoot->m_pLeft, k);

  if (target == nullptr) {
    if (k == 1)
      target = pRoot;
    k--;
  }

  if (target == nullptr && pRoot->m_pRight != nullptr)
    target = KthNodeCore(pRoot->m_pRight, k);

  return target;
}
const BiTreeNode *KthNode(const BiTreeNode *pRoot, unsigned int k) {
  if (pRoot == nullptr || k == 0)
    return nullptr;

  return KthNodeCore(pRoot, k);
}
// ====================测试代码====================
void Test(const char *testName, const BiTreeNode *pRoot, unsigned int k,
          bool isNull, int expected) {

  cout << "\n========================================\n" << testName << endl;

  const BiTreeNode *pTarget = KthNode(pRoot, k);
  if ((isNull && pTarget == nullptr) ||
      (!isNull && pTarget->m_nValue == expected))
    cout << "Passed.\n";
  else
    cout << "FAILED.\n";
}

void test() {
  //            8
  //        6      10
  //       5 7    9  11
  BiTreeNode *pNode8 = CreateBiTreeNode(8);
  BiTreeNode *pNode6 = CreateBiTreeNode(6);
  BiTreeNode *pNode10 = CreateBiTreeNode(10);
  BiTreeNode *pNode5 = CreateBiTreeNode(5);
  BiTreeNode *pNode7 = CreateBiTreeNode(7);
  BiTreeNode *pNode9 = CreateBiTreeNode(9);
  BiTreeNode *pNode11 = CreateBiTreeNode(11);

  ConnectBiTreeNodes(pNode8, pNode6, pNode10);
  ConnectBiTreeNodes(pNode6, pNode5, pNode7);
  ConnectBiTreeNodes(pNode10, pNode9, pNode11);

  Test("TestA0", pNode8, 0, true, -1);
  Test("TestA1", pNode8, 1, false, 5);
  Test("TestA2", pNode8, 2, false, 6);
  Test("TestA3", pNode8, 3, false, 7);
  Test("TestA4", pNode8, 4, false, 8);
  Test("TestA5", pNode8, 5, false, 9);
  Test("TestA6", pNode8, 6, false, 10);
  Test("TestA7", pNode8, 7, false, 11);
  Test("TestA8", pNode8, 8, true, -1);

  DestroyTree(pNode8);

} //! test
} //! Q35

namespace Q36 {
// 面试题41：数据流中的中位数
// 题目：如何得到一个数据流中的中位数？如果从数据流中读出奇数个数值，那么
// 中位数就是所有数值排序之后位于中间的数值。如果从数据流中读出偶数个数值，
// 那么中位数就是所有数值排序之后中间两个数的平均值。

// 使用两个优先队列/堆，分别保存小于中位数和大于中位数的两部分。

template <typename T> class CDynamicArray {
public:
  void insert(T item) {
    //当前是奇数个的时候，压入到lhs。首先压入到rhs，并从小堆rhs获取最小值，然后弹出，并压入到lhs。
    //当前是偶数个的时候，压入到rhs。首先压入到lhs，并从大堆lhs获取最大值，然后弹出，并压入到rhs。
    if (((lhs.size() + rhs.size()) & 0x01) == 0) {
      if (rhs.size() > 0 && less<T>()(item, rhs[0])) {
        rhs.push_back(item);
        push_heap(rhs.begin(), rhs.end(), less<T>());
        item = rhs[0];
        pop_heap(rhs.begin(), rhs.end(), less<T>());
        rhs.pop_back();
      }
      lhs.push_back(item);
      push_heap(lhs.begin(), lhs.end(), greater<T>());
    } else {
      if (lhs.size() > 0 && greater<T>()(item, lhs[0])) {
        lhs.push_back(item);
        push_heap(lhs.begin(), lhs.end(), greater<T>());
        item = lhs[0];
        pop_heap(lhs.begin(), lhs.end(), greater<T>());
        lhs.pop_back();
      }
      rhs.push_back(item);

      push_heap(rhs.begin(), rhs.end(), less<T>());
    }
  }

  T getMedian() {

    int sz = lhs.size() + rhs.size();
    if (sz != 0) {
      T median;
      if (sz & 0x01) {
        median = lhs[0];
      } else {
        median = myavg(lhs[0], rhs[0]);
      }
      return median;
    } else {
      throw runtime_error(string("No numbers are available"));
    }
  }

private:
  inline T myavg(T &a, T &b) { return (a + b) / 2.0; }
  vector<T> lhs; //大堆lhs保存小值.
  vector<T> rhs; //小堆rhs保存大值.
};
void Test(char *testName, CDynamicArray<double> &numbers, double expected) {

  cout << "\n========================================\n" << testName << endl;
  if (myequal(numbers.getMedian(), expected))
    cout << "Passed.\n";
  else
    cout << "FAILED.\n";
}

int test() {
  CDynamicArray<double> numbers;

  printf("Test1 begins: ");
  try {
    numbers.getMedian();
    cout << "FAILED.\n";
  } catch (const exception &e) {
    cout << "Passed. " << e.what() << endl;
  }

  numbers.insert(5);
  Test("Test2", numbers, 5);

  numbers.insert(2);
  Test("Test3", numbers, 3.5);

  numbers.insert(3);
  Test("Test4", numbers, 3);

  numbers.insert(4);
  Test("Test6", numbers, 3.5);

  numbers.insert(1);
  Test("Test5", numbers, 3);

  numbers.insert(6);
  Test("Test7", numbers, 3.5);

  numbers.insert(7);
  Test("Test8", numbers, 4);

  numbers.insert(0);
  Test("Test9", numbers, 3.5);

  numbers.insert(8);
  Test("Test10", numbers, 4);

  return 0;
}

} //! Q36
} //! CodingInterview
#endif // Q10_HPP
