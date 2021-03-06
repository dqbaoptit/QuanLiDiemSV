#include "dsmh.h"

/***************** Class MONHOC ********************/
MonHoc::MonHoc(char * mamh, char * tenmh, int stclt, int stcth) {
  strcpy(MAMH, mamh);
  strcpy(TENMH, tenmh);
  STCLT = stclt;
  STCTH = stcth;
}

/* *
 * Overriding operator >, <, == for class MonHoc
 */
bool MonHoc::operator > (MonHoc x) {
  return (strcmp(TENMH, x.TENMH) > 0);
}

bool MonHoc::operator < (MonHoc x) {
  return (strcmp(TENMH, x.TENMH) < 0);
}

bool MonHoc::operator == (MonHoc x) {
  return (strcmp(TENMH, x.TENMH) == 0);
}

/******************* Class node *********************/
node::node(MonHoc k) {
  key = k;
  height = 1;
  left = NULL;
  left = NULL;
}

/************* Class DanhSachMonHoc *****************/
DanhSachMonHoc::DanhSachMonHoc() {
  length = 0;
  root = NULL;
}

//---- Private methods
int DanhSachMonHoc::height(node * n) {
  /* * 
   * -> Return the height of a node 
   */
  if (n == NULL) {
    return 0;
  }
  return n->height;
}

node * DanhSachMonHoc::left_rotate(node * n) {
  /* *
   * Rotate a node to the left 
   * -> Return new node
   * */

  // Rotate
  node * new_n = n->right;
  n->right = new_n->left;
  new_n->left = n;

  // Update height
  n->height = 1 + std::max(height(n->left), height(n->right));
  new_n->height = 1 + std::max(height(new_n->left), height(new_n->right));
  
  return new_n;
}

node * DanhSachMonHoc::right_rotate(node * n) {
  /* *
   * Rotate a node to the right
   * -> Return new node
   * */
  
  // Rotate
  node * new_n = n->left;
  n->left = new_n->right;
  new_n->right = n;

  // Update height
  n->height = 1 + std::max(height(n->left), height(n->right));
  new_n->height = 1 + std::max(height(new_n->left), height(new_n->right));
  
  return new_n;
}

node * DanhSachMonHoc::insert_node(node * n, MonHoc x) {
  /* * 
   * Insert a node
   * then balance the tree
   * -> Return new node
   * */

  // Perform BST insert
  if (n == NULL) {
    length += 1;
    return new node(x); 
  } 
  if (x < n->key) {
    n->left= insert_node(n->left, x);
  } else if (x > n->key) {
    n->right= insert_node(n->right, x);
  } else {
    return new node(x);
  } 
  
  // Update height
  n->height = 1 + std::max(height(n->left), height(n->right));

  // Balance the tree
  int bal = height(n->left) - height(n->right);
  if (bal > 1) {
    if (x < n->left->key) {
      return right_rotate(n);
    } else {
      n->left = left_rotate(n->left);
      return right_rotate(n);
    }
  } else if (bal < -1) {
    if (x > n->right->key) {
      return left_rotate(n);
    } else {
      n->right = right_rotate(n->right);
      return left_rotate(n);
    }
  }

  return n; 
}

node * DanhSachMonHoc::remove_node(node * n, MonHoc x) {
  /* *
   * Remove a node from the tree
   * -> Return NULL if node is leave
   * otherwise, balance the tree and return new node
   */
  if (n == NULL) return n;  
  
  if (x < n->key) {  
    n->left = remove_node(n->left, x);  
  } else if (x > n->key) {  
    n->right = remove_node(n->right, x);  
  } else {  
    if((n->left == NULL) || (n->right == NULL)) {  
      node * temp = n->left ? n->left : n->right;  
      if (temp == NULL) {  
        temp = n;  
        n = NULL;  
      } else {   
        *n = *temp;   
      } 
      free(temp);  
    } else {   
      node * temp = n->right;
      while (temp->left != NULL) {
        temp = temp->left;
      }
      n->key = temp->key;  
      n->right = remove_node(n->right, temp->key);  
    }  
    length--;
  }  
  
  if (n == NULL) return n;  

  // Update height
  n->height = 1 + std::max(height(n->left), height(n->right));

  // Balance the tree
  int balance = height(n->left) - height(n->right); 
  // Left Left Case  
  if (balance > 1 && height(n->left->left) - height(n->left->right) >= 0)  
    return right_rotate(n);  
  // Left Right Case  
  if (balance > 1 &&  height(n->left->left) - height(n->left->right) < 0) {  
    n->left = left_rotate(n->left);  
    return right_rotate(n);  
  }  

  // Right Right Case  
  if (balance < -1 && height(n->right->left) - height(n->right->right) <= 0)  
    return left_rotate(n);  
  // Right Left Case  
  if (balance < -1 && height(n->right->left) - height(n->right->right) > 0) {
    n->right = right_rotate(n->right);  
    return left_rotate(n);  
  }  

  return n;  
}  

bool DanhSachMonHoc::search_name_node(node * n, char * s) {
  /* *
   * Search node by TENMH 
   * -> return TRUE if node exists
   */

  if (n == NULL) 
    return false;
  
  if (strcmp(s, n->key.TENMH) > 0) 
    return search_name_node(n->right, s);
  else if (strcmp(s, n->key.TENMH) < 0) 
    return search_name_node(n->left, s);
  else 
    return true;
}

void DanhSachMonHoc::in_order(node * n, std::function<void(MonHoc)> f) {
  /* *
   * Traverse through the tree and 
   * pass every single node to function f
   */

  if (n != NULL) {
    in_order(n->left, f);
    f(n->key);
    in_order(n->right, f);
  }
}

void DanhSachMonHoc::save_node(node * n, std::ofstream &f) {
  /* *
   * Save key of a node to file 
   */

  if (n != NULL) {
    save_node(n->left, f);
    f << n->key.MAMH << "\n" 
      << n->key.TENMH << "\n" 
      << n->key.STCLT << "\t" 
      << n->key.STCTH << "\n"; 
    save_node(n->right, f);
  }  
}

//---- Public methods

void DanhSachMonHoc::insert(MonHoc x) {
  root = insert_node(root, x);
}

void DanhSachMonHoc::remove(MonHoc x) {
  root = remove_node(root, x);    
}

bool DanhSachMonHoc::search_name(char * s) {
  return search_name_node(root, s);
}

bool DanhSachMonHoc::search_code(char * s) {
  bool found = false;
  enumerate([s, &found](MonHoc x) {
    if (strcmp(s, x.MAMH) == 0) found = true;
  });
  return found;
}

void DanhSachMonHoc::enumerate(std::function<void(MonHoc)> f) {
  in_order(root, f);
}

void DanhSachMonHoc::save() {
  std::ofstream f;
  f.open(db, std::ios::out);
  save_node(root, f);
  f.close();
}

void DanhSachMonHoc::load() {
  std::ifstream f;
  f.open(db, std::ios::in);

  std::string code_str;
  std::string name_str;
  int stclt;
  int stcth;
  while(getline(f,code_str)){
    getline(f,name_str);
    f>>stclt>>stcth;
    std::string tmp;
    getline(f,tmp);

    MonHoc curr_sub((char*) code_str.c_str(), (char*) name_str.c_str(),stclt, stcth);
    insert(curr_sub);
  }  

  f.close();
}
