#include "ce.h"

bool insert = false;

bool initializer(Buffer* message_buffer, Point* cursor)
{
     (void)(cursor);
     g_message_buffer = message_buffer;

     return true;
}

bool key_handler(int key, Buffer* buffer, Point* cursor)
{
     if(insert){
          // TODO: should be a switch
          if(key == 27){ // escape
               insert = false;
               if(buffer->lines[cursor->y]){
                    int64_t line_len = strlen(buffer->lines[cursor->y]);
                    if(cursor->x == line_len){
                         cursor->x--;
                    }
               }
          }else if(key == 127){ // backspace
               if(buffer->line_count){
                    if(cursor->x == 0){
                         if(cursor->y != 0){
                              // remove the line and join the next line with the previous
                         }
                    }else{
                         if(ce_remove_char(buffer, cursor)){
                              cursor->x--;
                         }
                    }
               }
          }else if(key == 10){ // add empty line
               if(!buffer->line_count){
                    ce_alloc_lines(buffer, 1);
               }
               if(ce_insert_newline(buffer, cursor->y + 1)){
                    cursor->y++;
                    cursor->x = 0;
               }
          }else{ // insert
               if(buffer->line_count == 0) ce_alloc_lines(buffer, 1);

               if(ce_insert_char(buffer, cursor, key)) cursor->x++;
          }
     }else{
          switch(key){
               default:
                    break;
               case 'q':
                    return false; // exit !
               case 'j':
               {
                    if(cursor->y < (buffer->line_count - 1)){
                         cursor->y++;
                    }

                    if(buffer->lines[cursor->y]){
                         int64_t line_length = strlen(buffer->lines[cursor->y]);

                         if(cursor->x >= line_length){
                              cursor->x = line_length - 1;
                              if(cursor->x < 0){
                                   cursor->x = 0;
                              }
                         }
                    }else{
                         cursor->x = 0;
                    }
               } break;
               case 'k':
               {
                    if(cursor->y > 0){
                         cursor->y--;
                    }

                    if(buffer->lines[cursor->y]){
                         int64_t line_length = strlen(buffer->lines[cursor->y]);

                         if(cursor->x >= line_length){
                              cursor->x = line_length - 1;
                              if(cursor->x < 0){
                                   cursor->x = 0;
                              }
                         }
                    }else{
                         cursor->x = 0;
                    }
               } break;
               case 'h':
               if(cursor->x > 0){
                    cursor->x--;
               }
               break;
               case 'l':
               {
                    if(buffer->lines[cursor->y]){
                         int64_t line_length = strlen(buffer->lines[cursor->y]);
                         if(cursor->x < (line_length - 1)){
                              cursor->x++;
                         }
                    }
               } break;
               case 'i':
                    insert = true;
                    break;
               case '':
                    if(buffer->lines[cursor->y]){
                         cursor->x++;
                    }
                    insert = true;
               case 'd':
                    // delete line
                    if(buffer->line_count){
                         if(ce_remove_line(buffer, cursor->y)){
                              if(cursor->y >= buffer->line_count){
                                   cursor->y = buffer->line_count - 1;
                              }
                         }
                    }
                    break;
               case 's':
                    ce_save_buffer(buffer, buffer->filename);
                    break;
          }
     }

     return true;
}