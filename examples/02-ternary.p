(
    ( fn conditional_double ( + $0 ( ? ( $1 ) ( $0 ) ( 0 ) ) ) )
    (  
       ( print ( !conditional_double 3 1 ) ) ( # should double )
       ( print ( !conditional_double 1 0 ) ) ( # should not double )

    )
)
