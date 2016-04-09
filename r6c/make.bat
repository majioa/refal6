call refc rcmain rsserv rclist
copy rcmain.rex+rsserv.rex+rclist.rex c.rex

call refc rccompn rcgen rcout
copy rccompn.rex+rcgen.rex+rcout.rex rccomp.rex

call refc rslex rssyn rslexn
copy rslex.rex+rssyn.rex rssynt.rex
copy rslexn.rex+rssyn.rex rssynt6.rex
