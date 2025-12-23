
( A_EQ 1 ( = 1 1 ) )
( A_EQ 0 ( = 2 1 ) )
( A_EQ 10 ( ? ( = 2 1 ) ( 0 ) ( 10 ) ) )

(
    ( fn recurse ( 
        ? ( = 1 $0 ) 
            ( 1 ) 
            ( !recurse ( - $0 1 ) ) 
             
        ) 
    ) 
    ( A_EQ 1 ( !recurse 3 ) )
)

