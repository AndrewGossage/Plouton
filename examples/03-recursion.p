 (
  ( # this is not working )
    ( fn recurse  ( ? ( $0 ) ( !recurse ( - $0 1 ) ) ( print 0 ) ) )
    ( !recurse 3 )
)
