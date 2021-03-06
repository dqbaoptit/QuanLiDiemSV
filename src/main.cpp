/************************************************/
// Init database
#include "./database/database.h"
Database database;

#include "./helpers/validator/validator.h"

// Core functions 
#include "./core/core.h"

// GUI
#include "./GUI/table.h"
#include "./GUI/menu.h"
#include "./GUI/form.h"

// Printing
#include "./print/print.h"

// Extra libs
#include <thread>
#include <chrono>

/************************************************/
// Chooses's code

// Main menu
const short CHOOSE_QLLTC = 1;
const short CHOOSE_QLLCQ = 2;
const short CHOOSE_QLMH = 3;
const short CHOOSE_NHAP_DIEM = 4;
const short CHOOSE_DANG_KY = 5;
const short CHOOSE_XEM_DIEM = 6;
const short CHOOSE_THOAT = 7;

// DSLTC, DSLCQ, DSMH menu
const short CHOOSE_THEM = 8;
const short CHOOSE_CHINH_SUA = 9;
const short CHOOSE_XOA = 10;
const short CHOOSE_QUAY_LAI = 11;

// Score board menu
const short CHOOSE_BANG_DIEM_KHOA_HOC = 12;
const short CHOOSE_BANG_DIEM_TONG_KET = 13;

// App's state
const short DSLTC = 1;
const short DSLCQ = 2;
const short DSMH = 3;
const short DSSV = 4;
const short DSDK = 5;
const short NHAP_DIEM_1 = 6;
const short NHAP_DIEM_2 = 7;
const short DANG_KI_1 = 8;
const short DANG_KI_2 = 9;
const short XEM_DIEM = 10;

/*************************************************/
class App {
private:
  // App state
  bool is_running;
  short choice, state;
  int input;

  // GUI members variables
  WINDOW * wins[3];
  Menu current_menu;
  Table current_table;
  Form current_form;
  
  // Private methods
  Form get_form();
  Table get_table();

  void render_menu(Menu);
  void render_table();
  void render_form();
  
  void set_picked_item();
  void set_buffer();
  
  void process_add();
  void process_update();
  void process_delete();
  
  void process_input();
  void process_menu();
  void process_form(FORM *);

  void notificate(char *);
public:
  App();
  void run();
  void exit();
};

App::App() {
  is_running = true;

  //-- Main screen
  initscr();
  clear();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  start_color();
  
  int row, column;
  getmaxyx(stdscr, row, column);

  //-- Menu window
  wins[0] = newwin(row - 1, (int)column/4, 0, 0);
  keypad(wins[0], TRUE);
  box(wins[0], 0, 0);
  wrefresh(wins[0]);
  //-- Main window
  wins[1] = newwin(row - 1, (int)(3*column)/4, 0, (int)column/4 + 1);
  keypad(wins[1], TRUE);
  box(wins[1], 0, 0);
  wrefresh(wins[1]);
  //-- Notification window
  wins[2] = newwin(3, (int)(3*column)/4 - 2, row - 5, (int)column/4 + 2);

  mvprintw(row - 1, 1, "ARROW KEY: Di chuyen       ENTER: Chon        ESC: Quay Lai Menu");
  mvprintw(row - 1, column - 25, "mccor-grimmz-conghau7620");
  refresh();
  current_menu = Menu(wins[0], 1);
}

Form App::get_form() {
  Form form(wins[1]);
  switch (state) {
    case DSLTC: 
      form.set_type(1);
      form.set_len(6);
      if (choice == CHOOSE_THEM) form.set_submit(add_loptc);
      if (choice == CHOOSE_CHINH_SUA) form.set_submit(update_loptc);
      form.set_validate(validate_ltc);
      break;
    case DSLCQ:
      form.set_type(2);
      form.set_len(1);
      form.set_submit(add_lopcq);
      form.set_validate(validate_lcq);
      break;
    case DSMH: 
      form.set_type(3);
      form.set_len(4);
      if (choice == CHOOSE_THEM) form.set_submit(add_mh);
      if (choice == CHOOSE_CHINH_SUA) form.set_submit(update_mh);
      form.set_validate(validate_mh);
      break;
    case DSSV:
      form.set_type(4);
      form.set_len(5);
      if (choice == CHOOSE_THEM) form.set_submit(add_sv);
      if (choice == CHOOSE_CHINH_SUA) form.set_submit(update_sv);
      form.set_validate(validate_sv);
      break;
    case NHAP_DIEM_1:
      form.set_type(5);
      form.set_len(4);
      form.set_submit(find_loptc);
      form.set_validate(validate_nhap_diem_1);
      break;
    case NHAP_DIEM_2:
      form.set_type(6);
      form.set_len(1);
      form.set_submit(set_score);
      form.set_validate(validate_nhap_diem_2);
      break;
    case DANG_KI_1:
      form.set_type(7);
      form.set_len(1);
      form.set_submit(search_sv);
      form.set_validate(validate_dang_ki_1);
      break;
    case DANG_KI_2:
      form.set_type(8);
      form.set_len(2);
      form.set_submit(filter_dsltc);
      form.set_validate(validate_dang_ki_2);
      break;
    default:
      break;
  }
  return form;
}

Table App::get_table() {
  Table table(wins[1]);
  switch (state) {
    case DSLTC:
      strcpy(table.title, "DANH SACH LOP TIN CHI");
      table.length = database.dsltc.getN();
      break;
    case DSLCQ:
      strcpy(table.title, "DANH SACH LOP CHINH QUY");
      table.length = database.dslcq.count();
      break;
    case DSMH:
      strcpy(table.title, "DANH SACH MON HOC");
      table.length = database.dsmh.length;
      break;
    case DSSV:
      strcpy(table.title, "DANH SACH SINH VIEN");
      table.length = database.get_current_dssv()->count();
      break;
    case DSDK:
      strcpy(table.title, "DANH SACH SINH VIEN DANG KY");
      table.length = database.get_current_dsdk()->count();
      break;
    case NHAP_DIEM_2:
      strcpy(table.title, "BANG DIEM DANH SACH DANG KY");
      table.length = database.get_current_dsdk()->count();
      break;
    case DANG_KI_2:
      strcpy(table.title, "BANG DIEM DANH SACH DANG KY");
      table.length = database.filtered_dsltc->getN();
      break;
    case XEM_DIEM:
      strcpy(table.title, "BANG DIEM TONG KET LOP CHINH QUY");
      table.length = database.get_current_dssv()->count();
      break;
    default:
      break;
  }
  return table;
}

void App::render_menu(Menu new_menu) {
  unpost_menu(current_menu.menu);
  current_menu = new_menu;
  current_menu.display();
}

void App::render_table() {
  wclear(wins[1]);
  switch (state) {
    case DSLTC: 
      print_dsltc(wins[1], current_table);
      break;
    case DSLCQ:
      print_dslcq(wins[1], current_table);
      break;
    case DSMH: 
      print_dsmh(wins[1], current_table);
      break;
    case DSSV:
      print_dssv(wins[1], current_table);
      break;
    case DSDK:
      print_dsdk(wins[1], current_table);
      break;
    case NHAP_DIEM_2:
      print_dsdk(wins[1], current_table);
      break;
    case DANG_KI_2:
      print_filtered_dsltc(wins[1], current_table);
      break;
    case XEM_DIEM:
      print_bang_diem_TK(wins[1], current_table);
      break;
    default:
      break;
  }
}

void App::set_picked_item() {
  switch (state) {
    case DSLTC:
      database.set_current_loptc(current_table.current_index);
      break;
    case DSLCQ:
      database.set_current_lopcq(current_table.current_index);
      break;
    case DSMH:
      database.set_current_mh(current_table.current_index);
      break;
    case DSSV:
      database.set_current_sv(current_table.current_index);
      break;
    case DSDK:
      database.set_current_svdk(current_table.current_index);
      break;
    case NHAP_DIEM_1:
    case NHAP_DIEM_2:
      database.set_current_svdk(current_table.current_index);
      break;
    case DANG_KI_1:
    case DANG_KI_2:
      database.set_current_loptc(database.filtered_dsltc->get_by_id(current_table.current_index));
      break;
    default:
      break;
  }
}

void App::set_buffer() {
  switch (state) {
    case DSLTC: 
      current_form.set_buffer_loptc(*database.get_current_loptc());
      break;
    case DSMH: 
      current_form.set_buffer_mh(database.get_current_mh());
      break;
    case DSSV:
      current_form.set_buffer_sv(database.get_current_sv()->get_data());
      break;
    default:
      break;
  }
}

void App::process_delete() {
  switch (state) {
    case DSLTC: 
      delete_loptc(); 
      break;
    case DSMH: 
      delete_mh();
      break;
    case DSDK:
      delete_svdk(current_table.current_index);
      break;
    case DSSV:
      delete_sv(current_table.current_index);
      break;
    default:
      break;
  }
}

void App::render_form() {
  wclear(wins[1]);
  current_form = get_form();
  current_form.display();
}

void App::process_menu() {
  switch (choice) {
    case CHOOSE_QLLTC: {
      state = DSLTC;
      render_menu(Menu(wins[0], 2));
      short pick;
      current_table = get_table();
      do {
        render_table();
        if (database.dslcq.count() == 0) break;
        pick = current_table.get_input();
        switch (pick) {
          case 1:
            // Picked
            set_picked_item();
            state = DSDK;
            current_table = get_table();
            do {
              render_table();
              pick = current_table.get_input();
              switch (pick) {
                case 1:
                  pick = 0;
                  break;
                case 3:
                  return;
              }
            } while (pick == 0);
            state = DSLTC;
            pick = 0;
            current_table = get_table();
            break;
          case 2:
            // Back
          case 3:
            // Exit
            return;
        }
      } while (pick == 0);
      break;
    }
    case CHOOSE_QLLCQ: {
      state = DSLCQ;
      render_menu(Menu(wins[0], 2));
      short pick;
      current_table = get_table();
      do {
        render_table();
        if (database.dslcq.count() == 0) break;
        pick = current_table.get_input();
        switch (pick) {
          case 1:
            // Picked
            set_picked_item();
            state = DSSV;
            current_table = get_table();
            do {
              render_table();
              pick = current_table.get_input();
              switch (pick) {
                case 1:
                  pick = 0;
                  break;
                case 3:
                  return;
              }
            } while (pick == 0);
            state = DSLCQ;
            pick = 0;
            current_table = get_table();
            break;
          case 2:
            // Back
          case 3:
            // Exit
            return;
        }
      } while (pick == 0);
      break;
    }
    case CHOOSE_QLMH: {
      state = DSMH;
      render_menu(Menu(wins[0], 2));
      short pick; 
      current_table = get_table();
      do {
        render_table();
        pick = current_table.get_input();

        switch (pick) {
          case 1:
            pick = 0;
            break;
          case 2:
          case 3:
            return;
        }
      } while (pick == 0);
      break;
    } 
    case CHOOSE_NHAP_DIEM: {
      state = NHAP_DIEM_1;
      render_form();
      
      // Phase 1: Get LopTC
      bool success;
      database.set_current_dsdk(NULL);
      success = current_form.process_input();
      
      // Phase 2: Nhap diem
      state = NHAP_DIEM_2;
      short pick; 
      if (success) {
        do {
          current_table = get_table();
          do {
            render_table();
            pick = current_table.get_input();
            switch (pick) {
              case 1:
                set_picked_item();
                render_form();
                success = current_form.process_input();
                if (success) notificate((char *)"Nhap diem thanh cong!");
                else notificate((char *)"Nhap diem khong thanh cong!");
                break;
              case 2:
              case 3:
                break;
            }
          } while (pick == 0);
        } while (pick != 3);
      } else {
        notificate((char *)"Lop khong ton tai!");
        wrefresh(wins[1]);
      }
      break;
    }

    case CHOOSE_DANG_KY: {
      int row, column;
      getmaxyx(wins[1], row, column);

      // Phase 1: Nhap MASV
      state = DANG_KI_1;
      database.set_current_sv(new Node<SinhVien>);

      render_form();
      bool found;
      found = current_form.process_input();
      
      if (!found) {
        notificate((char *)"Sinh vien khong ton tai!");
        break;
      } 

      // Phase 2: Loc LopTC
      state = DANG_KI_2;
      render_form();
      print_sv_info(wins[1], 1, 5, database.get_current_sv()->get_data());
      wrefresh(wins[1]);
      found = current_form.process_input();

      // Phase 3: Chon LopTC
      short pick;
      current_table = get_table();
      do {
        render_table();
        pick = current_table.get_input();
        switch (pick) {
          case 1: {
            set_picked_item();
            bool success = dang_ky(database.get_current_sv()->get_data().get_MASV());
            
            if (success) 
              notificate((char *)"Dang ki thanh cong!");
            else 
              notificate((char *)"Dang ki khong thanh cong!");

            pick = 0;
            break;
          }
          case 2:
          case 3:
            break;
        }
      } while (pick == 0);
      break;
    }

    case CHOOSE_XEM_DIEM: { 
      state = DSLCQ;
      render_menu(Menu(wins[0], 2));
      short pick;
      current_table = get_table();
      do {
        render_table();
        pick = current_table.get_input();

        switch (pick) {
          case 1:
            // Picked
            set_picked_item();
            if (database.get_current_dssv()->count() == 0) {
              notificate((char *)"Lop khong co sinh vien!");
              break;
            }
            state = XEM_DIEM;
            current_table = get_table();
            do {
              wclear(wins[1]);
              print_bang_diem_TK(wins[1], current_table);
              pick = current_table.get_input();
              switch (pick) {
                case 1:
                  pick = 0;
                  break;
                case 2:
                  break;
                case 3:
                  return;
              }
            } while (pick == 0);
            state = DSLCQ;
            pick = 0;
            current_table = get_table();
            break;

          case 2:
            // Back
          case 3:
            // Exit
            return;
        }
      } while (pick == 0);
      break;
    }

    case CHOOSE_THOAT:
      is_running = false;
      break;

    case CHOOSE_THEM: {
      render_form();
      bool valid;
      valid = current_form.process_input();
      if (valid)
        notificate((char *)"Them thanh cong!");
      else 
        notificate((char *)"Them khong thanh cong!");
      current_table = get_table();
      render_table();
      wrefresh(wins[1]);
      break;
    }

    case CHOOSE_CHINH_SUA: {
      short pick;
      do {
        render_table();
        pick = current_table.get_input();

        switch (pick) {
          case 1: {
            // Picked
            set_picked_item();
            render_form();
            set_buffer();
            bool success = current_form.process_input();
            if (success) {
              notificate((char *)"Chinh sua thanh cong!");
            } else {
              notificate((char *)"Chinh sua khong thanh cong!");
            }
            pick = 0;
            break;
          }
          case 2:
            // Back
          case 3:
            // Exit
            return;
        }
      } while (pick == 0);
      break;
    }

    case CHOOSE_XOA: {
      short pick;
      do {
        render_table();
        pick = current_table.get_input();

        switch (pick) {
          case 1:
            // Picked
            set_picked_item();
            process_delete(); 
            notificate((char *)"Xoa thanh cong!"); 
            current_table = get_table();
            wrefresh(wins[1]);
            pick = 0;
            break;

          case 2:
            // Back
          case 3:
            // Exit
            return;
        }
      } while (pick == 0);
      break;
    }
    case CHOOSE_QUAY_LAI:
      unpost_menu(current_menu.menu);
      current_menu = Menu(wins[0], 1);
      current_menu.display();
      break;
  }  
}

void App::notificate(char * message) {
  box(wins[2], 0, 0);
  mvwprintw(wins[2], 1, 1, message);
  wrefresh(wins[2]);
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  wclear(wins[2]);
}

void App::process_input() {
  while(input = wgetch(wins[0])) {
    switch (input) {
      case KEY_UP:
        menu_driver(current_menu.menu, REQ_UP_ITEM);
        break;  
      case KEY_DOWN:
        menu_driver(current_menu.menu, REQ_DOWN_ITEM);
        break;
      case KEY_RIGHT:
        current_table.get_input();
        break;
      case 10:
        switch (current_menu.type) {
          case 1:
            choice = item_index(current_item(current_menu.menu)) + 1;
            return;
          case 2:
            choice = item_index(current_item(current_menu.menu)) + 8;
            return;
          case 3:
            if (item_index(current_item(current_menu.menu)) == 2) choice = 11;
            else choice = item_index(current_item(current_menu.menu)) + 12;
            break;
        }
        return;
    }
  }
}

void App::run() {
  current_menu.display();
  wrefresh(wins[0]);
  wrefresh(wins[1]);
  refresh();
  while (is_running) {
    process_input();
    process_menu();
  }
  exit();
}

void App::exit() {
  wclear(wins[0]);
  wclear(wins[1]);
  clear();
  database.dsltc.save();
  database.dslcq.save();
  database.dsmh.save();
  free_menu(current_menu.menu);
  endwin();
}

/***************************************************/
int main() {
  App our_app;
  our_app.run();
}
