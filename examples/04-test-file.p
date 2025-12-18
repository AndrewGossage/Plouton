(
    ( fn conditional_double ( + $0 ( ? ( $1 ) ( $0 ) ( 0 ) ) ) )
    (  
      + ( !conditional_double 3 1 ) ( # should double )
        ( !conditional_double 1 0 ) ( # should not double )

    )
)
