( # basic tests )
( A_EQ 0 ( <  1  1 ) )
( A_EQ 0 ( <  2  1 ) )
( A_EQ 1 ( <  1  2 ) )
( A_EQ 1 ( < -1  2 ) )
( A_EQ 0 ( <  1 -1 ) )

( # advanced tests ) 
( fn min ( ? ( < $0 $1 ) ( $0 ) ( $1 ) ) )  
( fn max ( ? ( < $0 $1 ) ( $1 ) ( $0 ) ) )  
( A_EQ 3 ( !min 3 9 ) )  
( A_EQ 9 ( !max 3 9 ) )  




 
