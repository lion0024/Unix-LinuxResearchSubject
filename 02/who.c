/* who - /etc/utmpを読み出し、その中の情報のリストを表示する
 *       - 空レコードが出力されないようにし、時刻を適切に整形する
 */

#include <stdio.h>
#include <unistd.h>
#include <utmp.h>
#include <fcntl.h>
#include <time.h>
#include <stdlib.h>

/* #define SHOWHOST */

void showtime(long);
void show_info(struct utmp *);

int main(int argc, char *argv[])
{
	struct	utmp	utbuf;						/* 情報をここに読み出す					*/
	int						utmpfd;						/* このデスクリプタから読み出す	*/

  if (argc != 1 && argc != 3) {
    exit(EXIT_FAILURE);
  }

	if ((utmpfd = open(UTMP_FILE, O_RDONLY)) == -1){
		fprintf(stderr, UTMP_FILE);
		exit(EXIT_FAILURE);
	}

	while (read(utmpfd, &utbuf, sizeof(utbuf)) == sizeof(utbuf))
		show_info(&utbuf);
	close(utmpfd);
	exit(EXIT_SUCCESS);
}

/* 
 * show_info()
 * 		utmp構造体の内容を人間が読めるように表示する
 * 		レコードにユーザ名が含まれていない場合には何も表示しない
 */
void show_info(struct utmp *utbuf)
{
	if (utbuf->ut_type != USER_PROCESS)
		return;

	printf("%-8.8s", utbuf->ut_name);						/* ログイン名		*/
	printf(" ");																/* スペース			*/
	printf("%-8.8s", utbuf->ut_line);						/* tty					*/
	printf(" ");																/* スペース			*/
	showtime(utbuf->ut_time);										/* 時刻の表示		*/
#ifndef SHOWHOST
	if (utbuf->ut_host[0] != '\0')
		printf(" (%s)", utbuf->ut_host);					/* ホスト				*/
#endif
		printf("\n");															/* 改行					*/
}

/*
 * 人間が理解しやすい形式で時刻を表示する
 * ctimeを使って文字列を組み立ててから、その一部を抜き出す
 * 注意：%12.12sは文字列をchar12子分で出力し、
 * 長さを12バイト以下に制限する
 */
void showtime(long timeval)
{
	char	*cp;													/* 時刻のアドレスを保持するポインタ	*/

	cp = ctime(&timeval);								/* 時刻を文字列に変換する						*/
  																		/* 文字列は次の通り									*/
																			/* Mon Feb	4 00:46:40 EST 1991			*/
																			/* 0123456789012345.								*/
	printf("%12.12s", cp+4);						/* 位置4から12自分を抜き出す				*/
}

