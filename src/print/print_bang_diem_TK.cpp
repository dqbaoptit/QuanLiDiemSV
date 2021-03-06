#include "./print.h"

void print_bang_diem_TK(WINDOW * current_window, Table &current_table) {
  DanhSachSinhVien * current_dssv = database.get_current_dssv();
  
  LinkedList<char *> list_ma_mh;
  // Get list MonHoc
  Node<SinhVien> * current_sv = current_dssv->p_head_;
  while (current_sv != NULL) {
    // ......
    Node<int> * curr_malop = current_sv->get_data().DS_LOPTC->p_head_;
    while (curr_malop != NULL) {
      int index = database.dsltc.search(curr_malop->get_data());
      if (index < 0) {
        curr_malop = curr_malop->get_next();
      } else {
        LopTC * curr_ltc = database.dsltc.node[index];
        if (!list_ma_mh.is_exist(curr_ltc->maMH))
          list_ma_mh.push_back(curr_ltc->maMH);
        
        curr_malop = curr_malop->get_next();
      }
    }
    current_sv = current_sv->get_next();
  }
  
  // Print
  
  /*************** Print title, fields ******************/
  box(current_window, 0, 0);
  // Print title
  mvwprintw(current_window, 1, (current_table.width - strlen(current_table.title)) / 2, current_table.title);
  mvwhline(current_window, 2, 1, 0, current_table.width - 2);
  
  // Print STT, MA SV, HO TEN
  mvwprintw(current_window, 3, 1, "STT");
  mvwaddch(current_window, 2, 5, ACS_TTEE);
  mvwaddch(current_window, 3, 5, ACS_VLINE);
  mvwaddch(current_window, 4, 5, ACS_PLUS);

  mvwprintw(current_window, 3, 6, "MA SV");
  mvwaddch(current_window, 2, 21, ACS_TTEE);
  mvwaddch(current_window, 3, 21, ACS_VLINE);
  mvwaddch(current_window, 4, 21, ACS_PLUS);

  mvwprintw(current_window, 3, 22, "HO TEN");
  mvwaddch(current_window, 2, 58, ACS_TTEE);
  mvwaddch(current_window, 3, 58, ACS_VLINE);
  mvwaddch(current_window, 4, 58, ACS_PLUS);

  // Print MA MON HOC
  int current_xcoord = 58;
  int average_width = (current_table.width - 58) / (list_ma_mh.count() + 1);
  Node<char *> * current_ma_mh = list_ma_mh.p_head_;
  while (current_ma_mh != NULL) {
    mvwprintw(current_window, 3, current_xcoord + 1, current_ma_mh->get_data());
    current_xcoord += average_width;

    mvwaddch(current_window, 2, current_xcoord, ACS_TTEE);
    mvwaddch(current_window, 3, current_xcoord, ACS_VLINE);
    mvwaddch(current_window, 4, current_xcoord, ACS_PLUS);

    current_ma_mh = current_ma_mh->get_next();
  }
  mvwprintw(current_window, 3, current_xcoord + 1, "Diem TB");
  mvwhline(current_window, 4, 1, 0, current_table.width - 2);
  

  /****************** Print data *******************/
  int current_ycoord = 5;
  init_pair(1, COLOR_BLUE, COLOR_BLACK);
  
  int i = current_table.start_index;

  current_sv = current_dssv->get_node_by_index(i);
  while (current_sv != NULL) {
    if (i > current_table.end_index) break;
    SinhVien current_sv_data = current_sv->get_data();
    
    if (current_table.current_index == i)
      wattron(current_window, A_BOLD | COLOR_PAIR(1));

    // Print STT, MA SV, HO TEN
    mvwprintw(current_window, current_ycoord, 1, std::to_string(i + 1).c_str());
    mvwaddch(current_window, current_ycoord - 1, 5, ACS_TTEE);
    mvwaddch(current_window, current_ycoord, 5, ACS_VLINE);
    mvwaddch(current_window, current_ycoord + 1, 5, ACS_PLUS);

    mvwprintw(current_window, current_ycoord, 6, current_sv_data.get_MASV());
    mvwaddch(current_window, current_ycoord - 1, 21, ACS_TTEE);
    mvwaddch(current_window, current_ycoord, 21, ACS_VLINE);
    mvwaddch(current_window, current_ycoord + 1, 21, ACS_PLUS);

    mvwprintw(current_window, current_ycoord, 22, strcat(strcat(current_sv_data.get_HO(), " "), current_sv_data.get_TEN()));
    mvwaddch(current_window, current_ycoord - 1, 58, ACS_TTEE);
    mvwaddch(current_window, current_ycoord, 58, ACS_VLINE);
    mvwaddch(current_window, current_ycoord + 1, 58, ACS_PLUS);

    // Print seperators
    current_xcoord = 58;
    current_ma_mh = list_ma_mh.p_head_;
    while (current_ma_mh != NULL) {
      current_xcoord += average_width;

      mvwaddch(current_window, current_ycoord - 1, current_xcoord, ACS_TTEE);
      mvwaddch(current_window, current_ycoord, current_xcoord, ACS_VLINE);
      mvwaddch(current_window, current_ycoord + 1, current_xcoord, ACS_PLUS);
      current_ma_mh = current_ma_mh->get_next();
    }
    
    // Print DIEM MON HOC
    float total_score = 0;
    int total_mh = 0;
    Node<int> * current_ma_loptc = current_sv_data.DS_LOPTC->head();
    
    while (current_ma_loptc != NULL) {
      int index = database.dsltc.search(current_ma_loptc->get_data());
      if (index < 0) {
        current_ma_loptc = current_ma_loptc->get_next();
        continue;
      }
      total_mh++;
      LopTC * current_loptc = database.dsltc.get_by_id(database.dsltc.search(current_ma_loptc->get_data()));

      // Get X coord
      current_xcoord = 58;
      current_ma_mh = list_ma_mh.p_head_;
      while (current_ma_mh != NULL) {
        if (strcmp(current_ma_mh->get_data(), current_loptc->maMH) == 0) break;
        current_xcoord += average_width;
        current_ma_mh = current_ma_mh->get_next();
      }

      // Get DIEM
      Node<SinhVienDK> * current_svdk = current_loptc->dsdk->head();
      while (current_svdk != NULL) {
        if (strcmp(current_svdk->get_data().get_MASV(), current_sv_data.get_MASV()) == 0) {
          std::string diem = std::to_string(current_svdk->get_data().get_DIEM());
          diem.resize(4);
          // Print
          mvwprintw(current_window, current_ycoord, current_xcoord + 1, diem.c_str());
          total_score += current_svdk->get_data().get_DIEM();
          break;
        }
        current_svdk = current_svdk->get_next();
      }
      current_ma_loptc = current_ma_loptc->get_next();
    }

    // Print DIEM TRUNG BINH
    float average_score = total_score / total_mh;
    std::string average_score_str = std::to_string(average_score);
    average_score_str.resize(4);
    mvwprintw(current_window, current_ycoord, 59 + average_width * list_ma_mh.count(), average_score_str.c_str());
    
    if (current_table.current_index == i)
      wattroff(current_window, A_BOLD | COLOR_PAIR(1));

    // Next
    mvwhline(current_window, current_ycoord + 1, 1, 0, current_table.width - 2);
    i++;
    current_ycoord += 2;
    current_sv = current_sv->get_next();
  }

  wrefresh(current_window);
}
