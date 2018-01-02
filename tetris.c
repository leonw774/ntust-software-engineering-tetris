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
int can_fall( void ); // check if the block can fall
int can_left( void );
int can_right( void );
int can_rotate( int cp, int cr );
void del_lines( void ); // delete full lines
int isgameover( int piece, int rotation ); // check if game is over

/* On board, 0: space, 1: falling block, 2: rotation core, 3: piling block, 4: border */

char BOARD[ 21 ][ 12 ] = {0}; // 20x10 without borders
char PIECES[ 7 ][ 4 ][ 4 ][ 4 ] = // 7 kinds, 4 rotations, stored in 4x4
{
	{ // O
		{
			{ 0, 0, 0, 0 },
			{ 0, 1, 2, 0 },
			{ 0, 1, 1, 0 },
			{ 0, 0, 0, 0 }
		},
		{
			{ 0, 0, 0, 0 },
			{ 0, 1, 2, 0 },
			{ 0, 1, 1, 0 },
			{ 0, 0, 0, 0 }
		},
		{
			{ 0, 0, 0, 0 },
			{ 0, 1, 2, 0 },
			{ 0, 1, 1, 0 },
			{ 0, 0, 0, 0 }
		},
		{
			{ 0, 0, 0, 0 },
			{ 0, 1, 2, 0 },
			{ 0, 1, 1, 0 },
			{ 0, 0, 0, 0 }
		}
	}, // end O
	{ // I
		{
			{ 0, 0, 0, 0 },
			{ 1, 1, 2, 1 },
			{ 0, 0, 0, 0 },
			{ 0, 0, 0, 0 }
		},
		{
			{ 0, 0, 1, 0 },
			{ 0, 0, 2, 0 },
			{ 0, 0, 1, 0 },
			{ 0, 0, 1, 0 }
		},
		{
			{ 0, 0, 0, 0 },
			{ 1, 1, 2, 1 },
			{ 0, 0, 0, 0 },
			{ 0, 0, 0, 0 }
		},
		{
			{ 0, 0, 1, 0 },
			{ 0, 0, 2, 0 },
			{ 0, 0, 1, 0 },
			{ 0, 0, 1, 0 }
		}
	}, // end I
	{ // S
		{
			{ 0, 0, 0, 0 },
			{ 0, 0, 2, 1 },
			{ 0, 1, 1, 0 },
			{ 0, 0, 0, 0 }
		},
		{
			{ 0, 0, 1, 0 },
			{ 0, 0, 2, 1 },
			{ 0, 0, 0, 1 },
			{ 0, 0, 0, 0 }
		},
		{
			{ 0, 0, 0, 0 },
			{ 0, 0, 2, 1 },
			{ 0, 1, 1, 0 },
			{ 0, 0, 0, 0 }
		},
		{
			{ 0, 0, 1, 0 },
			{ 0, 0, 2, 1 },
			{ 0, 0, 0, 1 },
			{ 0, 0, 0, 0 }
		}
	}, // end S
	{ // Z
		{
			{ 0, 0, 0, 0 },
			{ 0, 1, 2, 0 },
			{ 0, 0, 1, 1 },
			{ 0, 0, 0, 0 }
		},
		{
			{ 0, 0, 0, 1 },
			{ 0, 0, 2, 1 },
			{ 0, 0, 1, 0 },
			{ 0, 0, 0, 0 }
		},
		{
			{ 0, 0, 0, 0 },
			{ 0, 1, 2, 0 },
			{ 0, 0, 1, 1 },
			{ 0, 0, 0, 0 }
		},
		{
			{ 0, 0, 0, 1 },
			{ 0, 0, 2, 1 },
			{ 0, 0, 1, 0 },
			{ 0, 0, 0, 0 }
		}
	}, // end Z
	{ // L
		{
			{ 0, 0, 0, 0 },
			{ 0, 1, 2, 1 },
			{ 0, 1, 0, 0 },
			{ 0, 0, 0, 0 }
		},
		{
			{ 0, 0, 1, 0 },
			{ 0, 0, 2, 0 },
			{ 0, 0, 1, 1 },
			{ 0, 0, 0, 0 }
		},
		{
			{ 0, 0, 0, 1 },
			{ 0, 1, 2, 1 },
			{ 0, 0, 0, 0 },
			{ 0, 0, 0, 0 }
		},
		{
			{ 0, 1, 1, 0 },
			{ 0, 0, 2, 0 },
			{ 0, 0, 1, 0 },
			{ 0, 0, 0, 0 }
		}
	}, // end L
	{ // J
		{
			{ 0, 0, 0, 0 },
			{ 0, 1, 2, 1 },
			{ 0, 0, 0, 1 },
			{ 0, 0, 0, 0 }
		},
		{
			{ 0, 0, 1, 1 },
			{ 0, 0, 2, 0 },
			{ 0, 0, 1, 0 },
			{ 0, 0, 0, 0 }
		},
		{
			{ 0, 1, 0, 0 },
			{ 0, 1, 2, 1 },
			{ 0, 0, 0, 0 },
			{ 0, 0, 0, 0 }
		},
		{
			{ 0, 0, 1, 0 },
			{ 0, 0, 2, 0 },
			{ 0, 1, 1, 0 },
			{ 0, 0, 0, 0 }
		}
	}, // end J
	{ // T
		{
			{ 0, 0, 0, 0 },
			{ 0, 1, 2, 1 },
			{ 0, 0, 1, 0 },
			{ 0, 0, 0, 0 }
		},
		{
			{ 0, 0, 1, 0 },
			{ 0, 0, 2, 1 },
			{ 0, 0, 1, 0 },
			{ 0, 0, 0, 0 }
		},
		{
			{ 0, 0, 1, 0 },
			{ 0, 1, 2, 1 },
			{ 0, 0, 0, 0 },
			{ 0, 0, 0, 0 }
		},
		{
			{ 0, 0, 1, 0 },
			{ 0, 1, 2, 0 },
			{ 0, 0, 1, 0 },
			{ 0, 0, 0, 0 }
		}
	} // end T
}; // end PIECES

int main( void )
{
	char input;
	BOARD_init();
	int t, cp, cr, np, nr; // declare counter, current piece/rotation and next
	srand( time(NULL) );
	np = rand() % 7, nr = rand() % 4; // initialize next piece	
	int block_falling = 0;
	while ( 1 ) {
		if (block_falling == 0) { // every block colliding, update board and new block
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
						BOARD_init();
						srand( time(NULL) );
						np = rand() % 7, nr = rand() % 4;
						continue;
				} else break;
			} // end if
			block_init( cp, cr );
			block_falling = 1;
		} // end if
		else {
			input = getche();
			switch ( input ) {
				case 's':
					if ( can_fall() == 1 ) block_fall();
					break;
					case 'a':
					if ( can_left() == 1 ) block_left();
					break;
				case 'd':
					if ( can_right() == 1 ) block_right();
					break;
				case 'w':
					if ( can_rotate( cp, cr ) == 1 ) {
						cr = ( cr + 1 ) % 4; // rotate the piece
						block_rotate( cp, cr ); // overwrite with the rotated piece
					} break;
				case ' ':
					block_land();
					break;
			} // end switch 
			if ( can_fall() == 1 )
				block_fall();
			else{
				block_falling = 0;
				block_turn(); // turn falling block into piling block
				del_lines();
			}
		} // end else
		sleep( 4/5 ); // loop per second
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
				if ( i == 1 ) printf( "　　俄羅斯方塊 ver1.01 by 郭家銍"  );
				else if ( i == 4 ) printf( "　　Bug1. 未解決定時落下與等候輸入之衝突" );
				else if ( i == 5 ) printf( "　　      必須透過按鍵輸入使程式繼續執行" );
				else if ( i == 7 ) printf( "　　Bug2. 未解決輸入字串的尾端多餘之\"\\n\"" );
				else if ( i == 8 ) printf( "　　      任何按鍵輸入都會使方塊落下一格" );
				else if ( i == 10 ) printf( "　　Bug3. 未解決函數 getche() 之輸入回顯" );
				else if ( i == 11 ) printf( "　　      導致按鍵輸入會出現在畫面中一瞬" );
				else if ( i == 13 ) printf( "　　〈操作說明〉※須切換至英文輸入" );
				else if ( i == 15 ) printf( "　　Enter: 下落一格　　Space: 下落到底" );
				else if ( i == 17 ) printf( "　　S: 下落兩格　　　　W: 旋轉" );
				else if ( i == 19 ) printf( "　　A: 左移　　　　　　D: 右移" );
				printf( "\n" );
			} else {
				if ( BOARD[ i ][ j ] == 0 ) printf( "　" );
				else if ( BOARD[ i ][ j ] == 1 || BOARD[ i ][ j ] == 2 ) printf( "＊" );
				else if ( BOARD[ i ][ j ] == 3 ) printf( "＃" );
			} // end else
		} // end for
	} // end for
	printf( "　　　└──────────┘" ); // print bottom border
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

int can_rotate( int cp, int cr )
{
	int i, j, core_i, core_j;
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
	if ( cp == 1 ) { // if current piece is I, should detect 4x4
		for ( i = core_i - 1; i <= core_i + 2; i++ ) {
			for ( j = core_j - 2; j <= core_j + 1; j++ ) {
				if ( BOARD[ i ][ j ] > 2 ) return 0;
			} // end for
		} // end for
		return 1;
	} else { // other piece, should detect 3x3
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
	int i, j, core_i, core_j;
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
	} else { // other piece except O, overwrite 3x3
		for ( i = core_i - 1; i <= core_i + 1; i++ ) {
			for ( j = core_j - 1; j <= core_j + 1; j++ ) {
				BOARD[ i ][ j ] = PIECES[ cp ][ cr ][ i - core_i + 1 ][ j - core_j + 2 ];
			} // end for
		} // end for
	} // end else
}

void block_land( void )
{
	int i, j, upper, bottom, distance = 0;
	int min_distance = 40;
	for ( j = 1; j < 11; j++ ) {
		upper = 0;
		bottom = 20;
		for ( i = 0; i < 20; i++ ) 
			if ( BOARD[ i ][ j ] >= 1 && BOARD[ i ][ j ] <= 2 && upper < i ) upper = i;
		for ( i = 20; i >= 0; i-- ) 
			if ( BOARD[ i ][ j ] >= 3 && BOARD[ i ][ j ] <= 4 && bottom > i ) bottom = i;
		distance = bottom - upper - 1;
		if ( min_distance > distance ) min_distance = distance;
	} // end for
	if ( min_distance != 0 ) {
		for ( i = 19; i >= 0 ; i-- ) { 
			for ( j = 1; j < 11; j++ ) {
				if ( BOARD[ i ][ j ] == 1 || BOARD[ i ][ j ] == 2 ) {
					BOARD[ i + min_distance ][ j ] = BOARD[ i ][ j ]; 	// replace the bottom line with current line
					BOARD[ i ][ j ] = 0; // clean the current line
				}
			}
		}
	}
	display();
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
