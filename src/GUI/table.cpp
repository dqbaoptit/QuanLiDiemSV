#include "./table.h"

char * DSLTC_FIELDS[] = { 
  "MA MH",
  "NIEN KHOA",
  "HOC KI",
  "NHOM",
  "SO SV MIN",
  "SO SV MAX",
  "HUY",
};

char * DSLCQ_FIELDS[] = {
  "MA LOP",
  "SO SV"
};

char * DSMH_FIELDS[] = {
  "MA MH",
  "TEN MH",
  "STCLT",
  "STCTH",
};

char * DSSV_FIELDS[] = {
  "MA SV",
  "HO",
  "TEN",
  "GIOI TINH",
  "SDT",
};

char * DSDK_FIELDS[] = {
  "MA SV",
  "DIEM",
};

Table::Table(WINDOW * win) {
  current_window = win;
  getmaxyx(win, height, width);
  page_size = width - 5;
  current_line = 0;
  input = 0;
}

bool Table::get_input() {
  input = getch();
  if (input == KEY_F(1)) {
    return false;
  }

 process_input();
  return true;
}

void Table::process_input() {
  int top_char;
  int bottom_char;

  switch (input) {
    case KEY_UP:
      current_line--;
      break;
    case KEY_DOWN:
      current_line++;
      break;
  }
}

void Table::display() {
  box(current_window, 0, 0);
  char ** fields;
  switch (type) {
    case 1:
      fields = DSLTC_FIELDS;
      len = 7; 
      break;
    case 2:
      fields = DSLCQ_FIELDS;
      len = 2;
      break;
    case 3:
      fields = DSMH_FIELDS;
      len = 4;
      break;
    case 4:
      fields = DSSV_FIELDS;
      len = 5;
      break;
    case 5:
      fields = DSDK_FIELDS;
      len = 2;
      break;
  }
  
  int current_xCoord = 0;
  // Title
  mvwprintw(current_window, 1, (width - strlen(title)) / 2, title);
  mvwhline(current_window, 2, 1, 0, width - 2);
  mvwhline(current_window, 4, 1, 0, width - 2);
    
  // Fields
  average_width = width / len;
  for (int i = 0; i < len; i++) {
    mvwprintw(current_window, 3, current_xCoord + 1, fields[i]);
    mvwvline(current_window, 5, current_xCoord, 0, height - 6);
    
    if (i > 0) {
      mvwaddch(current_window, 2, current_xCoord, ACS_TTEE);
      mvwaddch(current_window, 3, current_xCoord, ACS_VLINE);
      mvwaddch(current_window, 4, current_xCoord, ACS_PLUS);
    }

    current_xCoord += average_width + 1;
  }
  wrefresh(current_window);
}

void Table::render_dsltc(DanhSachLopTC& dsltc) {
  int length = dsltc.getN();  
  
  int current_yCoord = 5;
  for (int i = 0; i < length; i++) {
    LopTC * curr_loptc = dsltc.node[i];
    mvwprintw(current_window, current_yCoord, 1 + average_width * 0, curr_loptc->maMH);
    mvwprintw(current_window, current_yCoord, 1 + average_width * 1, curr_loptc->nienkhoa);
    mvwprintw(current_window, current_yCoord, 1 + average_width * 2, std::to_string(curr_loptc->hocki).c_str());
    mvwprintw(current_window, current_yCoord, 1 + average_width * 3, std::to_string(curr_loptc->nhom).c_str());
    mvwprintw(current_window, current_yCoord, 1 + average_width * 4, std::to_string(curr_loptc->sv_min).c_str());
    mvwprintw(current_window, current_yCoord, 1 + average_width * 5, std::to_string(curr_loptc->sv_max).c_str());

    mvwhline(current_window, current_yCoord + 1, 1, 0, width - 2);
    current_yCoord += 2;
    wrefresh(current_window);
  }
}

void Table::render_dslcq(DanhSachLopCQ dslcq) {

}

void Table::render_dsmh(DanhSachMonHoc dsmh) {
  int length = dsmh.length;
  int current_yCoord = 5;
  int i = 0;
  dsmh.enumerate([i, current_yCoord, this](MonHoc x) mutable{
    mvwprintw(current_window, current_yCoord, 1 + (average_width + 1)*0, x.MAMH);
    mvwprintw(current_window, current_yCoord, 1 + (average_width + 1)*1, x.TENMH);
    mvwprintw(current_window, current_yCoord, 1 + (average_width + 1)*2, std::to_string(x.STCLT).c_str());
    mvwprintw(current_window, current_yCoord, 1 + (average_width + 1)*3, std::to_string(x.STCLT).c_str());
    
    i++;
    mvwhline(current_window, current_yCoord + 1, 1, 0, width - 2);
    current_yCoord += 2;
  });
  wrefresh(current_window);
}

void Table::render_dssv(DanhSachSinhVien dssv) {

}

void Table::render_dsdk(DanhSachSinhVienDK dsdk) {

}
