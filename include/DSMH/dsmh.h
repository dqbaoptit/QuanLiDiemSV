#ifndef _DSMH_H
#define _DSMH_H
#include <iostream>
#include <fstream>
#include <cstring>
#include <functional>

#include "../../src/helpers/helpers.h"

/********** MON HOC as a KEY of a node *********/ 
class MonHoc {
public:
  // Properties 
  char MAMH[10];   
  char TENMH[50];
  int STCLT;
  int STCTH;
  
  // Constructor 
  MonHoc(char* , char*, int, int);
  MonHoc() {;}

  // Operator overloading 
  bool operator>(MonHoc);
  bool operator<(MonHoc);
  bool operator==(MonHoc);
};

/*************** AVL TREE NODE ****************/
class node {
public:
  // Properties 
  MonHoc key;
  int height;
  node *left;
  node *right;

  // Constructor 
  node(MonHoc);
};

/***************** AVL TREE *******************/
class DanhSachMonHoc {
public:
  // Properties
  int length;
  node * root; 
  char db[32] = "../database/dsmh.txt";

  // Constructor
  DanhSachMonHoc();
 
  /* * 
   * Public methods contain methods that manipulate the 
   * list directly and hide the logic under the hood.  
   */ 
  void insert(MonHoc);
  void remove(MonHoc);
  void update(MonHoc, MonHoc);

  bool search_name(char *);
  bool search_code(char *);
  
  void enumerate(std::function<void(MonHoc)>); 
  
  // File IO
  void load();
  void save();

private:
  /* *
   * Private methods contain methods that interract with
   * the AVL tree and being called by public methods.    
   */
  int height(node *);
  
  node * left_rotate(node *);
  node * right_rotate(node *);
  
  node * insert_node(node *, MonHoc);
  node * remove_node(node *, MonHoc);

  bool search_name_node(node *, char *);

  void in_order(node *, std::function<void(MonHoc)>);
  void save_node(node *, std::ofstream &);
};
#endif
