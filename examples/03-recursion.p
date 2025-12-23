(
    ( fn recurse ( ( print $0 ) ( ? ( $0 ) ( !recurse ( - $0 1 ) ) ( 0 ) ) ) ) 
    ( !recurse 3 )
)
