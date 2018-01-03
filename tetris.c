// written by 郭家銍 四電資三 B10432030 in 2015
// last time modified in 2018
 
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

void display( void ); // refresh the board
void BOARD_init( void ); // generate a new board
void block_init( int piece, int rotation ); // generate a new block
void block_fall( void ); // make the block fall
void block_turn( void );
void block_left( void );
void block_right( void );
void block_rotate( int cp, int cr );
void block_land( void );
int can_fall( void ); // check if the block can fall
int can_left( void );
int can_right( void );
int can_rotate( int cp, int cr, int clockwise );
void del_lines( void ); // delete full lines
int isgameover( int piece, int rotation ); // check if game is over

/* On board, 0: space, 1: falling block, 2: rotation core, 3: piling block, 4: border */
class Model
{
	public int total_del = 0, difficulty = 1, speed = 1000; // total of deleted lines
	public char [ , , ] BOARD = new char[ 21 , 12 ] {0}; // 20x10 without borders
	public char [ , , , ] PIECES = new char[ 7 , 4 , 4 , 4 ]// 7 kinds, 4 rotations, stored in 4x4
	{
	    { // O
	        {
                    { '0', '0', '0', '0' },
                    { '0', '1', '2', '0' },
                    { '0', '1', '1', '0' },
                    { '0', '0', '0', '0' }
                },
                {
                    { '0', '0', '0', '0' },
                    { '0', '1', '2', '0' },
                    { '0', '1', '1', '0' },
                    { '0', '0', '0', '0' }
                },
                {
                    { '0', '0', '0', '0' },
                    { '0', '1', '2', '0' },
                    { '0', '1', '1', '0' },
                    { '0', '0', '0', '0' }
                },
                {
                    { '0', '0', '0', '0' },
                    { '0', '1', '2', '0' },
                    { '0', '1', '1', '0' },
                    { '0', '0', '0', '0' }
                }
            }, // end O
	    { // I
		{
                    { '0', '0', '0', '0' },
                    { '1', '1', '2', '1' },
                    { '0', '0', '0', '0' },
                    { '0', '0', '0', '0' }
                },
                {
                    { '0', '0', '1', '0' },
                    { '0', '0', '2', '0' },
                    { '0', '0', '1', '0' },
                    { '0', '0', '1', '0' }
                },
                {
                    { '0', '0', '0', '0' },
                    { '1', '1', '2', '1' },
                    { '0', '0', '0', '0' },
                    { '0', '0', '0', '0' }
                },
                {
                    { '0', '0', '1', '0' },
                    { '0', '0', '2', '0' },
                    { '0', '0', '1', '0' },
                    { '0', '0', '1', '0' }
                }
            }, // end I
    	    { // S
		{
                    { '0', '0', '0', '0' },
                    { '0', '0', '2', '1' },
                    { '0', '1', '1', '0' },
                    { '0', '0', '0', '0' }
                },
                {
                    { '0', '0', '1', '0' },
                    { '0', '0', '2', '1' },
                    { '0', '0', '0', '1' },
                    { '0', '0', '0', '0' }
                },
                {
                    { '0', '0', '0', '0' },
                    { '0', '0', '2', '1' },
                    { '0', '1', '1', '0' },
                    { '0', '0', '0', '0' }
                },
                {
                    { '0', '0', '1', '0' },
                    { '0', '0', '2', '1' },
                    { '0', '0', '0', '1' },
                    { '0', '0', '0', '0' }
                }
           }, // end S
	   { // Z
		{
                    { '0', '0', '0', '0' },
                    { '0', '1', '2', '0' },
                    { '0', '0', '1', '1' },
                    { '0', '0', '0', '0' }
                },
                {
                    { '0', '0', '0', '1' },
                    { '0', '0', '2', '1' },
                    { '0', '0', '1', '0' },
                    { '0', '0', '0', '0' }
                },
                {
                    { '0', '0', '0', '0' },
                    { '0', '1', '2', '0' },
                    { '0', '0', '1', '1' },
                    { '0', '0', '0', '0' }
                },
                {
                    { '0', '0', '0', '1' },
                    { '0', '0', '2', '1' },
                    { '0', '0', '1', '0' },
                    { '0', '0', '0', '0' }
                }
           }, // end Z
	   { // L
		{
                    { '0', '0', '0', '0' },
                    { '0', '1', '2', '1' },
                    { '0', '1', '0', '0' },
                    { '0', '0', '0', '0' }
                },
                {
                    { '0', '0', '1', '0' },
                    { '0', '0', '2', '0' },
                    { '0', '0', '1', '1' },
                    { '0', '0', '0', '0' }
                },
                {
                    { '0', '0', '0', '1' },
                    { '0', '1', '2', '1' },
                    { '0', '0', '0', '0' },
                    { '0', '0', '0', '0' }
                },
                {
                    { '0', '1', '1', '0' },
                    { '0', '0', '2', '0' },
                    { '0', '0', '1', '0' },
                    { '0', '0', '0', '0' }
                }
           }, // end L
	   { // J
		{
                    { '0', '0', '0', '0' },
                    { '0', '1', '2', '1' },
                    { '0', '0', '0', '1' },
                    { '0', '0', '0', '0' }
                },
                {
                    { '0', '0', '1', '1' },
                    { '0', '0', '2', '0' },
                    { '0', '0', '1', '0' },
                    { '0', '0', '0', '0' }
                },
                {
                    { '0', '1', '0', '0' },
                    { '0', '1', '2', '1' },
                    { '0', '0', '0', '0' },
                    { '0', '0', '0', '0' }
                },
                {
                    { '0', '0', '1', '0' },
                    { '0', '0', '2', '0' },
                    { '0', '1', '1', '0' },
                    { '0', '0', '0', '0' }
                }
           }, // end J
	   { // T
		{
                    { '0', '0', '0', '0' },
                    { '0', '1', '2', '1' },
                    { '0', '0', '1', '0' },
                    { '0', '0', '0', '0' }
                },
                {
                    { '0', '0', '1', '0' },
                    { '0', '0', '2', '1' },
                    { '0', '0', '1', '0' },
                    { '0', '0', '0', '0' }
                },
                {
                    { '0', '0', '1', '0' },
                    { '0', '1', '2', '1' },
                    { '0', '0', '0', '0' },
                    { '0', '0', '0', '0' }
                },
                {
                    { '0', '0', '1', '0' },
                    { '0', '1', '2', '0' },
                    { '0', '0', '1', '0' },
                    { '0', '0', '0', '0' }
                }
           } // end T
}; // end PIECES
}


int main( void )
{
	char input;
	BOARD_init(); // initialize the board
	int cp, cr, np, nr; // declare current piece/rotation and next
	clock_t t1 = clock(), t2 = clock(); // declare timer
	srand( time(NULL) );
	np = rand() % 7, nr = rand() % 4; // initialize next piece	
	int start = 0, block_falling = 0, landing = 0;
	while ( 1 ) { // looping eternally
		t2 = clock(); // count passing time
		while ( ( int )( t2 - t1 ) >= speed || start++ == 0 || block_falling == 0  ) { // initially start and activate after every second passing
			if ( block_falling == 0 ) { // if block piling, update board and new block
				cp = np; // replace current piece with next piece
				cr = nr;
				np = rand() % 7; // randomize next piece
				nr = rand() % 4;
				if ( isgameover( cp, cr ) == 1 ) {
					system( "CLS" );
					printf( "\n\n　　　　Game Over! Play again? (y/n) " );
					input = getche();
					while ( input != 'y' && input != 'n' ) {
						system( "CLS" );
						printf( "\n\n　　　　Game Over! Play again? (y/n) " );
						input = getche();
					} // end while
					if ( input == 'y' ) {
							start = 0;
							BOARD_init();
							srand( time(NULL) );
							np = rand() % 7, nr = rand() % 4;
							total_del = 0;
							difficulty = 1;
							speed = 1000;
							continue;
					} else return 0;
				} // end if
				block_init( cp, cr );
				block_falling = 1;
			} else { // block falling or colliding
				if ( can_fall() == 1 )
					block_fall();
				else{
					block_falling = 0;
					block_turn(); // turn falling block into piling block
					del_lines();
				} // end else
			} // end else
			t1 = clock(); // reset the timer;
			t2 = clock();
		} // end while
		while ( kbhit() == 1 ) { // activate when input detected
			input = getch();
			switch ( input ) {
				case 's':
					if ( can_fall() == 1 ) block_fall();
					t2 = clock();
					break;
				case 'a':
					if ( can_left() == 1 ) block_left();
					break;
					t2 = clock();
				case 'd':
					if ( can_right() == 1 ) block_right();
					t2 = clock();
					break;
				case 'w': // counter clockwise 
					if ( can_rotate( cp, cr, 1 ) == 1 ) {
						cr = ( cr + 1 ) % 4; // rotate the piece
						block_rotate( cp, cr ); // overwrite with the rotated piece
					} // end if
					t2 = clock();
					break;
				case 'e': // clockwise
					if ( can_rotate( cp, cr, -1 ) == 1 ) {
						if ( cr == 0 ) cr = 4;
						else cr -= 1; // rotate the piece
						block_rotate( cp, cr ); // overwrite with the rotated piece
					} // end if
					t2 = clock();
					break;
				case ' ':
					while ( can_fall() == 1 ) block_land();
					landing = 1;
					display();
					t2 = clock();
					break;
			} // end switch

		} // end while
		t2 = clock(); // count passing time
	} // end while
}

void display( void )
{
	int i, j; // counter
	system( "CLS" ); // refresh the screen
	printf( "\n" );
	for ( i = 0; i < 20; i++ ) {
		printf( "　　　" );
		for ( j = 0; j < 12; j++ ) {
			if ( j == 0 ) printf( "│" ); // print left border
			else if ( j == 11 ) {
				printf( "│" ); // print right border
				if ( i == 1  ) printf( "　　俄羅斯方塊 ver2.04 by 郭家銍"			);
				if ( i == 3  ) printf( "　　Update.修正ver2.03中的Bug"		);
				if ( i == 7  ) printf( "　　〈操作說明〉※須切換至英文輸入"			);
				if ( i == 9  ) printf( "　　S: 下移　　　　    W: 旋轉" );
				if ( i == 11 ) printf( "　　A: 左移　　　　　　D: 右移");
				if ( i == 13 ) printf( "　　Space: 落下" );
				if ( i == 17 ) printf( "　　難易度：%d", difficulty );
				if ( i == 19 ) printf( "　　消除行數：%d", total_del );
				printf( "\n" );
			} else {
				if ( BOARD[ i ][ j ] == 0 ) printf( "　" );
				else if ( BOARD[ i ][ j ] == 1 || BOARD[ i ][ j ] == 2 ) printf( "＊" );
				else if ( BOARD[ i ][ j ] == 3 ) printf( "＃" );
			} // end else
		} // end for
	} // end for
	printf( "　　　└－－－－－－－－－－┘" ); // print bottom border
}

void BOARD_init( void )
{
	int i, j;
	for ( i = 0; i < 21; i++ )
		for ( j = 0; j < 12; j++ ) {
			if ( i == 20 ) BOARD[ i ][ j ] = 4;
			else if ( j == 0 || j == 11 ) BOARD[ i ][ j ] = 4;
			else BOARD[ i ][ j ] = 0;
		}
	display();
}


void block_init( int piece, int rotation )
{
	int i, j;
	for ( i = 0; i < 4; i++)
		for ( j = 0; j < 4; j++)
			if ( BOARD[ i ][ j + 3 ] == 0 )
				BOARD[ i ][ j + 3 ] = PIECES[ piece ][ rotation ][ i ][ j ];
	display();
}

int can_fall( void )
{
	int i, j;
	for ( i = 19; i >= 0 ; i-- ) { 
		for ( j = 1; j < 11; j++ ) {
			if ( BOARD[ i ][ j ] >= 1 && BOARD[ i ][ j ] <= 2  && BOARD[ i + 1 ][ j ] > 2 ) return 0;
		}
	}
	return 1;
}

void block_fall( void )
{
	int i, j;
	for ( i = 19; i >= 0 ; i-- ) { 
		for ( j = 1; j < 11; j++ ) {
			if ( BOARD[ i ][ j ] == 1 || BOARD[ i ][ j ] == 2 ) {
				BOARD[ i + 1 ][ j ] = BOARD[ i ][ j ]; 	// replace the next line with current line
				BOARD[ i ][ j ] = 0; // clean the current line
			}
		}
	} 
	display();
}

void block_turn( void )
{
	int i, j;
	for ( i = 19; i >= 0 ; i-- ) { 
		for ( j = 1; j < 11; j++ ) {
			if ( BOARD[ i ][ j ] == 1 || BOARD[ i ][ j ] == 2 ) BOARD[ i ][ j ] = 3;
		} 
	}
	display();
}

int can_left( void )
{
	int i, j;
	for ( i = 19; i >= 0 ; i-- ) { 
		for ( j = 1; j < 11; j++ ) {
			if ( BOARD[ i ][ j ] >= 1 && BOARD[ i ][ j ] <= 2 && BOARD[ i ][ j - 1 ] > 2 ) return 0;
		}
	}
	return 1;
}

void block_left( void )
{
	int i, j;
	for ( i = 19; i >= 0 ; i-- ) { 
		for ( j = 1; j < 11; j++ ) {
			if ( BOARD[ i ][ j ] == 1 || BOARD[ i ][ j ] == 2 ) {
				BOARD[ i ][ j - 1 ] = BOARD[ i ][ j ];
				BOARD[ i ][ j ] = 0;
			}
		}
	} 
	display();
}

int can_right( void )
{
	int i, j;
	for ( i = 19; i >= 0 ; i-- ) { 
		for ( j = 10; j > 0; j-- ) {
			if ( BOARD[ i ][ j ] >= 1 && BOARD[ i ][ j ] <= 2 && BOARD[ i ][ j + 1 ] > 2 ) return 0;
		}
	}
	return 1;
}

void block_right( void )
{
	int i, j;
	for ( i = 19; i >= 0 ; i-- ) { 
		for ( j = 10; j > 0; j-- ) {
			if ( BOARD[ i ][ j ] == 1 || BOARD[ i ][ j ] == 2 ) {
				BOARD[ i ][ j + 1 ] = BOARD[ i ][ j ];
				BOARD[ i ][ j ] = 0;
			}
		}
	} 
	display();
}

int can_rotate( int cp, int cr, int clockwise )
{
	if ( cr == 4 && clockwise == 1) cr = -1;
	if ( cr == 0 && clockwise == -1) cr = 5;
	int i, j, core_i, core_j, ti, tj;
	if ( cp == 0 ) return 0; // if current piece is O, return false
	for ( i = 19; i >= 0 ; i-- ) { // search the piece core
		for ( j = 1; j < 11; j++ ) {
			if ( BOARD[ i ][ j ] == 2 ) {
				core_i = i;
				core_j = j;
				break;
			} // end if
		} // end for
	} // end for
	if ( cp == 1 ) { // if current piece is I, detect 4x4
		for ( i = core_i - 1; i <= core_i + 2; i++ ) {
			for ( j = core_j - 2; j <= core_j + 1; j++ ) {
				if ( BOARD[ i ][ j ] > 2 ) return 0;
			} // end for
		} // end for
		return 1;
	} else if (cp == 6) { // else if current piece is T, should detect 3x3
		for ( i = core_i - 1, ti = 0; i <= core_i + 1; i++, ti++ ) {
			for ( j = core_j - 1, tj = 1; j <= core_j + 1; j++, tj++ ) {
				if ( PIECES[ cp ][ cr + clockwise ][ ti ][ tj ] > 0 && BOARD[ i ][ j ] > 2 ) return 0; // detect rule: T spin
			} // end for
		} // end for
		return 1;
	} else { // other pieces, detect 3x3
		for ( i = core_i - 1; i <= core_i + 1; i++ ) {
			for ( j = core_j - 1; j <= core_j + 1; j++ ) {
				if ( BOARD[ i ][ j ] > 2 ) return 0;
			} // end for
		} // end for
		return 1;
	} // end else
}

void block_rotate( int cp, int cr )
{
	int i, j, core_i, core_j, ti, tj;
	for ( i = 19; i >= 0 ; i-- ) { // search the piece core
		for ( j = 1; j < 11; j++ ) {
			if ( BOARD[ i ][ j ] == 2 ) {
				core_i = i;
				core_j = j;
				break;
			} // end if
		} // end for
	} // end for
	if ( cp == 1 ) { // if current piece is I , overwrite 4x4
		for ( i = core_i - 1; i <= core_i + 2; i++ ) {
			for ( j = core_j - 2; j <= core_j + 1; j++ ) {
				BOARD[ i ][ j ] = PIECES[ cp ][ cr ][ i - core_i + 1 ][ j - core_j + 2 ];
			} // end for
		} // end for
	} else if ( cp == 6 ) { // if current piece is T, overwrite cross only
		for ( i = core_i - 1, ti = 0; i <= core_i + 1; i++, ti++ ) {
			for ( j = core_j - 1, tj = 1; j <= core_j + 1; j++, tj++ ) {
				if ( PIECES[ cp ][ cr ][ ti ][ tj ] > 0 || BOARD[ i ][ j ] == 1 )
					BOARD[ i ][ j ] = PIECES[ cp ][ cr ][ i - core_i + 1 ][ j - core_j + 2 ];
			} // end for
		} // end for
	} else { // other piece except O, overwrite 3x3
		for ( i = core_i - 1; i <= core_i + 1; i++ ) {
			for ( j = core_j - 1; j <= core_j + 1; j++ ) {
				BOARD[ i ][ j ] = PIECES[ cp ][ cr ][ i - core_i + 1 ][ j - core_j + 2 ];
			} // end for
		} // end for
	} // end else
	display();
}

void block_land( void )
{
	int i, j;
	for ( i = 19; i >= 0 ; i-- ) { 
		for ( j = 1; j < 11; j++ ) {
			if ( BOARD[ i ][ j ] == 1 || BOARD[ i ][ j ] == 2 ) {
				BOARD[ i + 1 ][ j ] = BOARD[ i ][ j ]; 	// replace the next line with current line
				BOARD[ i ][ j ] = 0; // clean the current line
			}
		}
	} 
}

void del_lines( void )
{
	int i, j, d, del; // counter of height, width, delete or not, which row to start delete
	int rows = 0; // row number of del lines
	for ( i = 19; i >= 0 ; i-- ) {
		d = 1;
		for ( j = 1; j < 11; j++ ) {
			if ( BOARD[ i ][ j ] != 3 ) d = 0;
		} // end for
		if ( rows > 0 && d == 0 ) break;
		else {
			rows += d;
			del = i;
		} // end else
	} // end for
	del = del + rows - 1;
	for ( i = del; i >= rows ; i-- ) {
		for ( j = 1; j < 11; j++ ) {
			BOARD[ i ][ j ] = BOARD[ i - rows ][ j ]; 
		} // end for
	} 
	total_del += rows;
	if ( total_del < 5 ) {
		difficulty = 1;
		speed = 1000;
	} else if ( total_del < 10 ) {
		difficulty = 2;
		speed = 800;
	} else if ( total_del < 15 ) {
		difficulty = 3;
		speed = 600;
	} else if ( total_del < 20 ) {
		difficulty = 4;
		speed = 400;
	} else if ( total_del < 25 ) {
		difficulty = 5;
		speed = 200;
	} // end else if
	display();
}

int isgameover( int piece, int rotation )
{
	int i, j; // counter
	for ( i = 0; i < 4; i++ )
		for ( j = 0; j < 4; j++)
			if ( BOARD[ i ][ j + 3 ] == 3 && PIECES[ piece ][ rotation ][ i ][ j ] >= 1 )
				return 1;
	return 0;
}
