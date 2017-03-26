/* load file of user to array_user */
void load_user();
/* check if there is another user by this username */
int check_user();
/* check the username and password*/
int check_pass();
/* terminates the client connection */
void quit();
/* register the client */
int reg();
/* tell user a message */
int tell();
/* shout a message */
int shout();
/* change the password of client */
int change_password();
/* show the users online */
int who();
/* send an email */
int mail();
/* list the header of email */
int list_mail();
/* read a specefic email */
int read_mail();
/* delete a specefic email */
int del_mail();
/* count the unread email of user to when he login show him  */
int unread_mail();
/* initialize a match */
int init_match(char[],char[],int,int);
/* get opposite color */
char opposite(char);
/* return id of an online user */
int is_online(char[]);
/* return number of current games */
int game();
/* draw game board for users and observers */
void draw_game(int);
/* observe a game */
int observe(int,int);
/* unobserve a game */
int unobserve(int);
/* find an observing game */
int find_game_obs(int);
/* find a playing game */
int find_game_playing(int);
/* refresh a game */
int refresh(int);
/* check if a symbol wins */
int check_win(int,char);
/* show the all commands */
void help();
/* empty cur game board */
void empty_board(int);
/* resign from a game */
void resign(int,int);
/* kibitz and ' msg while observing */
void kibitz(int,int,char[]);
/* show the stats of user */
void stats(char[],int);
/* save the game results to file */
void save_result(int,int);
/* change info */
void info(int,char[]);
