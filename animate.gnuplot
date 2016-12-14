splot sprintf("Fin%i.dat",i)  using 1:2:3 title sprintf("t=%i",i)  pt 7 
i=i+1
if (i < n) reread