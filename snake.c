#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <time.h>
struct termios orig_termios;
typedef struct {
    int number;
    int x;
    int y;
} part;
void add_object(part** arr,int* count,int x, int y) {
    (*count)++;
    *arr = realloc(*arr, (*count) * sizeof(part));
    if (!*arr) {
        perror("Error with memory allocation!!!");
        exit(1);
    }
    (*arr)[*count - 1].x = x;
    (*arr)[*count - 1].y = y;
}
void end(int result){
        printf("Final result:%d",result);
}
void disable_raw_mode() {
    tcsetattr(STDIN_FILENO, TCSANOW, &orig_termios);
}

void enable_raw_mode() {
    tcgetattr(STDIN_FILENO, &orig_termios);
    atexit(disable_raw_mode); 

    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO); 
    raw.c_cc[VMIN] = 0;  
    raw.c_cc[VTIME] = 0; 
    tcsetattr(STDIN_FILENO, TCSANOW, &raw);
}

void print_game(int x,int y,int poz_x,int poz_y,int a_x,int a_y,int count,part* objects){
    int flag = 0;
    for(int i = 0;i<y;i++){
        for(int i2 = 0;i2<x;i2++){
            flag = 0;
            if (poz_x == i2 && poz_y == i){
                putchar('0');
            } else if (a_x == i2 && a_y == i){
                putchar('ó');
            } else {
                for (int j = 0;j<count;j++){
                    if (objects[j].x == i2 && objects[j].y == i){
                        putchar('o');
                        flag = 1;
                                                break;
                    }
                }
                if (!flag){
                    putchar('.');
                }
                        }
                }
                putchar('\n');
    }
        printf("Points:%d\n",count);

}
int Random(int min, int max) {
    return rand() % (max - min + 1) + min;
}

int main () {
        part* objects = NULL;
        enable_raw_mode();
        srand(time(NULL));
        int count = 0;
        int x = 50; //width
        int y = 35;     //height
        int poz_x_old = 0;
        int poz_y_old = 0;
        int x_a = Random(5, 10); //apple x
        int y_a = Random(5, 30); //apple y
        int poz_x = Random(5, 10);
        int poz_y = Random(7, 20);
        int running = 1;
        int up = 0;
        int down = 0;
        int right = 0;
        int left = 0;
    while (running) {
        char ch;
        if (read(STDIN_FILENO, &ch, 1) > 0) {
            if (ch == 27) { 
                char seq[2];
                read(STDIN_FILENO, &seq[0], 1);
                read(STDIN_FILENO, &seq[1], 1);
                if (seq[0] == '[') {
                    if (seq[1] == 'A' && down !=1) {up = 1;down = 0;right = 0;left = 0;}
                    if (seq[1] == 'B' && up != 1) {up = 0;down = 1;right = 0;left = 0;}
                    if (seq[1] == 'C' && left != 1) {up = 0;down = 0;right = 1;left = 0;}
                    if (seq[1] == 'D'&& right != 1) {up = 0;down = 0;right = 0;left = 1;}
                }
            }
            if (ch == 'q') running = 0; // q = exit
        }
                if (poz_y > y || poz_y < 1 || poz_x > x || poz_x < 1) {
                        printf("Game OVER!!!\n");
                        end(count);
                        //for (int i = 0;i < count; i++){
                        //      printf("x:%d y:%d\n",objects[i].x,objects[i].y);
                        //}
                        running = 0;
                        break;
                }
                if (up == 1){
                        poz_y--;
                }
                if (down == 1){
                        poz_y++;
                }
                if (right == 1){
                        poz_x++;
                }
                if (left == 1) {
                        poz_x--;
                }
                for (int i = 0; i < count; i++) {
                    if (poz_x == objects[i].x && poz_y == objects[i].y) {
                        printf("Game OVER!!!\n");
                                end(count);
                                exit(0);
                         }
                }

                if (x_a == poz_x &&  y_a == poz_y) {
                        if (count > 0) {
                                 add_object(&objects,&count,objects[count-1].x,objects[count-1].y);
                        } else {add_object(&objects,&count,poz_x,poz_y);}

                        x_a = Random(5, x-5);
                        y_a = Random(5, y-5);
                }

        print_game(x,y,poz_x,poz_y,x_a,y_a,count,objects);
                usleep(80000);
                system("clear");


                if (count > 0) {
    
        int prev_x = objects[0].x;
        int prev_y = objects[0].y;

        objects[0].x = poz_x;
        objects[0].y = poz_y;

        if (count > 1) {
        for (int i = 1; i < count; i++) {
                int temp_x = objects[i].x;
                int temp_y = objects[i].y;

                objects[i].x = prev_x;
                objects[i].y = prev_y;

                prev_x = temp_x;
                prev_y = temp_y;
                 }
                }
    }     
    }

free(objects);
return 0;
}

