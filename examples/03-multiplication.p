( # everything is a function including comments  )

(
    ( # consts are functions  )
    ( fn y ( 2 ) )

    ( # functions are functions  )
    ( fn x ( * $0 ( !y ) ) )

    ( # expressions are functions )
    ( + ( !x ( !y ) ) ( / 1 2) )
)
