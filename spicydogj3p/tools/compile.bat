set STDOUT=..\temp\stdout
call ant -f %1 -Dproperties="%2" compile 2>%STDOUT%
perl tools/javapp.pl %STDOUT%
