#include "lib/bt-5.0.0/inc/btree.h"
#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


GtkWidget *window, *aboutdialog, *adddialog, *okaboutdialog, *addbuttondialog, *textviewadd;
GtkListStore *allfile, *history;
GtkWidget *searchtab, *textview, *suggest, *thongbao, *searchentry, *recententry;
GtkWidget *addbutton, *undobutton, *editbutton, *redobutton, *deletebutton,   *thongbaoadd;
int i = 0;
void menuAbout() { gtk_widget_show_all(aboutdialog); }
void okAboutDialog() { gtk_widget_hide(aboutdialog); }
void RestoreDataFile() {
  remove("data/tudienanhviet.dat");
  FILE *ori = fopen("source/tudienanhviet.old", "rb");
  FILE *cur = fopen("source/tudienanhviet.dat", "wb");
  int n;
  char buffer[512];
  while (!feof(ori)) {
    n = fread(buffer, 1, 512, ori);
    if (n == 0)
      break;
    fwrite(buffer, 1, n, cur);
  }
  fclose(ori);
  fclose(cur);
}

void menuRestore() {
  RestoreDataFile();
  GtkTextBuffer *textbuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview));
  gtk_entry_set_text(GTK_ENTRY(searchtab), "");
  gtk_entry_set_text(GTK_ENTRY(searchentry), "");
  gtk_entry_set_text(GTK_ENTRY(recententry), "");
  gtk_text_buffer_set_text(textbuffer, "", -1);
  gtk_label_set_text(GTK_LABEL(thongbao), "Restore thanh cong");

}
void delete() {
  btinit();
  BTA *book;
  book = btopn("source/tudienanhviet.dat", 0, 0);
  if(book == NULL)
    printf("Khong mo duoc file\n");
  gchar *textSearch;
  GtkTextBuffer *textbuffer;
  textbuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview));
  BTint value;
  textSearch = gtk_entry_get_text(GTK_ENTRY(searchtab));

  if(bfndky(book,textSearch, &value) == 0){
      if(btdel(book, textSearch) == 0)
	gtk_label_set_text(GTK_LABEL(thongbao), "Xóa thành công");
    } else {
      gtk_label_set_text(GTK_LABEL(thongbao), "Không thể xóa từ này");
    }
  gtk_text_buffer_set_text(textbuffer, "", -1);
  btcls(book);
    
}
void key_enter() {
  btinit();
  BTA *book = btopn("source/tudienanhviet.dat", 0, 0);
  gchar *textSearch;
  gchar *text;
  GtkTreeIter iter;
  GtkTextBuffer *textbuffer;
  char val[6500];
  int rsize;
  textSearch = gtk_entry_get_text(GTK_ENTRY(searchtab));
  textbuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview));
  if(i == 1) {
    text = gtk_entry_get_text(GTK_ENTRY(searchentry));
    gtk_entry_set_text(GTK_ENTRY(recententry), text);
  } 
  if (strcmp(textSearch, "") == 0){
    gtk_text_buffer_set_text(textbuffer, "", -1);
  }
  else {
    if (btsel(book, textSearch, val, 6500, &rsize) == 0) {
      gtk_list_store_append(history, &iter);
      gtk_list_store_set(history, &iter, 0, textSearch, -1);
      gtk_text_buffer_set_text(textbuffer, val, -1);
      gtk_label_set_text(GTK_LABEL(thongbao), "Success");
      gtk_entry_set_text(GTK_ENTRY(searchentry), textSearch);
      i = 1;
    } else {
      gtk_label_set_text(GTK_LABEL(thongbao), "Dữ liệu không tìm thấy");
      gtk_text_buffer_set_text(textbuffer, "Not Found!!!", -1);   
    }
  }
  btcls(book);
}
void redo() {
 
}
void undo() {
}

void addShowDialog() {
  gtk_widget_show(adddialog);
  gtk_label_set_text(GTK_LABEL(thongbaoadd), "Nhap du lieu\n");
  
  
}

void cancelAddDialog() {
  gtk_widget_hide(adddialog);

}
void add() {
  BTA *book;
  btinit();
  book = btopn("source/tudienanhviet.dat", 0, 0);
  GtkTextIter start, end;
  GtkTreeIter iter;
  BTint rsize;
  gchar *text, *btext;
  GtkTextBuffer *textbuffer; 
  textbuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textviewadd));
  text = gtk_entry_get_text(GTK_ENTRY(searchtab));
  gtk_text_buffer_get_bounds(textbuffer, &start, &end);
  btext = gtk_text_buffer_get_text(textbuffer, &start, &end, FALSE);
  printf("%s \t %s \n", text, btext);
  if(bfndky(book, text, &rsize) == 0) {
    btupd(book, text, btext, 2000);
    gtk_label_set_text(GTK_LABEL(thongbaoadd), "Update thanh cong ");
  } else {
    if(btins(book, text, btext, 2000) == 0)
      gtk_label_set_text(GTK_LABEL(thongbaoadd), "Add thanh cong ");
    gtk_list_store_append(allfile, &iter);
    gtk_list_store_set(allfile, &iter, 0, text, -1);
  }
 
  btcls(book);
}
int main(int argc, char *argv[]) {
  GtkBuilder *builder;
  GtkTreeIter iter;
  GError *error = NULL;
  gtk_init(&argc, &argv);
  builder = gtk_builder_new();
  gtk_builder_add_from_file(builder, "gui.glade", NULL);
  window = GTK_WIDGET(gtk_builder_get_object(builder, "window")); // window
  aboutdialog = GTK_WIDGET(gtk_builder_get_object(
      builder, "aboutdialog")); // bat cua so about dialog

  adddialog = GTK_WIDGET(
      gtk_builder_get_object(builder, "adddialog")); // bat cua so add dialog
  btinit();
  searchtab = GTK_WIDGET(
      gtk_builder_get_object(builder, "searchtab")); // Thanh tim kiem
  textview = GTK_WIDGET(gtk_builder_get_object(builder, "textview"));
  suggest = GTK_WIDGET(gtk_builder_get_object(builder, "suggest"));
  thongbao = GTK_WIDGET(gtk_builder_get_object(builder, "thongbao"));
  searchentry = GTK_WIDGET(gtk_builder_get_object(builder, "searchentry"));
  recententry = GTK_WIDGET(gtk_builder_get_object(builder, "recententry"));
  addbutton = GTK_WIDGET(gtk_builder_get_object(builder, "add"));
  undobutton = GTK_WIDGET(gtk_builder_get_object(builder, "undo"));
  redobutton = GTK_WIDGET(gtk_builder_get_object(builder, "redo"));
  deletebutton = GTK_WIDGET(gtk_builder_get_object(builder, "delete"));
  addbuttondialog = GTK_WIDGET(gtk_builder_get_object(builder, "addbuttondialog"));
  thongbaoadd = GTK_WIDGET(gtk_builder_get_object(builder, "thongbaoadd"));
  textviewadd = GTK_WIDGET(gtk_builder_get_object(builder, "textviewadd"));
  allfile = GTK_LIST_STORE(gtk_builder_get_object(builder, "allfile"));
  history = GTK_LIST_STORE(gtk_builder_get_object(builder, "history"));
  char key[100];
  char val[1000];
  int rsize;
  BTA *book;
  book = btopn("source/tudienanhviet.dat", 0, 0);
  btpos(book, 1);
  while (btseln(book, key, val, 1000, &rsize) == 0) {
    gtk_list_store_append(allfile, &iter);
    gtk_list_store_set(allfile, &iter, 0, key, -1);
  }
  btcls(book);
  gtk_builder_connect_signals(builder, window);
  g_object_unref(G_OBJECT(builder));

  gtk_widget_show_all(window);

  gtk_main();
  return 1;
}
