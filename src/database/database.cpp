#include "database.h"

void Database::set_current_loptc(int index) {
  if (dsltc.node[index] != NULL) {
    current_loptc = dsltc.node[index];  
    current_dsdk = current_loptc->dsdk;
  }
}

void Database::set_current_lopcq(int index) {
  if (index > dslcq.count()) {
    current_lopcq = NULL;
    return;
  }

  current_lopcq = dslcq.p_head_;
  int i = 0;
  while (current_lopcq != NULL && i < index) {
    current_lopcq = current_lopcq->get_next();
    i++;
  }
  current_dssv = current_lopcq->get_data().DSSV;
  if (current_dssv->count() != 0) 
    current_dssv->sort();
}

void Database::set_current_lopcq(char * ma_lopcq) {
  current_lopcq = NULL;
    
  Node<LopCQ> * curr_node = dslcq.p_head_;
  while (curr_node != NULL) {
    if (strcmp(curr_node->get_data().MALOP, ma_lopcq) == 0) {
      current_lopcq = curr_node;
      current_dssv = current_lopcq->get_data().DSSV;
      break;
    }
    curr_node = curr_node->get_next();
  }
}

void Database::set_current_mh(int index) {
  int i = -1;
  dsmh.enumerate([&i, index, this](MonHoc x) {
    i++;
    if (i == index) {
      this->set_current_mh(x);
    } 
  });
}

void Database::set_current_sv(int index) {
  if (index > current_dssv->count()) {
    current_sv = NULL;
    return;
  }

  int i = 0;
  current_sv = current_dssv->p_head_;
  while (current_sv != NULL && i < index) {
    current_sv = current_sv->get_next();
    i++;
  }
}

void Database::set_current_sv(char * ma_sv) {
  current_sv = NULL;
  current_lopcq = dslcq.p_head_;
  
  Node<SinhVien> * current_sv_node;
  while (current_lopcq != NULL) {
    current_sv_node = current_lopcq->get_data().DSSV->p_head_;
    while (current_sv_node != NULL) {
      if (strcmp(current_sv_node->get_data().get_MASV(), ma_sv) == 0) {
        current_sv = current_sv_node;
        return;
      }
      current_sv_node = current_sv_node->get_next();
    }
    current_lopcq = current_lopcq->get_next();
  }
}

void Database::set_current_svdk(int index) {
  if (index > current_dsdk->count()) {
    current_svdk = NULL;
    return;
  }

  int i = 0;
  current_svdk = current_dsdk->p_head_;
  while (current_svdk != NULL && i < index) {
    current_svdk = current_svdk->get_next();
    i++;
  }
}

