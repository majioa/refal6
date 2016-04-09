*  mapinfo = (type location actulness) .
*  type = E-type | T-type .
*  E-type = expr | vector | string .
*  T-type = elemptr | pconst | char | int | headptr .
*  location = name | TO-STK | FROM-STK .
*  actualness = A | V .
NEW {
   sW  [ sW : ("elemptr" W V) , "rb_new_sym()" ,
          ("headptr*" STK A) : sR ] = sR ;
   sW sT = <TABLE_VALUE sT 'U' sW> ;
   };

(New (sw) (sr)
   (CC (sw ("headptr"  TO-STK V))
       "rb_new_sym()"
       (sr ("headptr*" TO-STK A))
   )
 )

(Make-slide (sR sL sT sR sB) ()
    (CC
        (sL ("int" TO-STK))
        (sT ("int" TO-STK))
        (sR ("int" TO-STK))
        (sB ("int" TO-STK))
        (sR ("headptr" TO-STK V))
        "CCALL(rs_mk_slide());"
     )
)
(Slide (sL sT sR sB) (sR)
    (CC
        (sL ("int" TO-STK))
        (sT ("int" TO-STK))
        (sR ("int" TO-STK))
        (sB ("int" TO-STK))
        "CCALL(rf_mk_hbox(&H)); pushp(H);"
        "CCALL(rs_mk_slide());"
        (sR ("headptr" H A))
     )
)


(Make-slide (sR sL sT sR sB) ()
    (CC
        (sL ("int" L))
        (sT ("int" T))
        (sR ("int" R))
        (sB ("int" B))
        (sR ("headptr" H V))
        "CCALL(rf_mk_slide(H,L,T,R,B));"
     )
)
(Slide (sL sT sR sB) (sR)
    (CC
        (sL ("int" L))
        (sT ("int" T))
        (sR ("int" R))
        (sB ("int" B))
        "CCALL(rf_mk_hbox(&H));"
        "CCALL(rf_mk_slide(H,L,T,R,B));"
        (sR ("headptr" H A))
     )
)



