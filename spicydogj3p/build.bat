call tools/compile.bat build.xml properties\jphone-f.properties
call tools/compile.bat build.xml properties\jphone-fm.properties
call tools/compile.bat build.xml properties\jphone-g.properties
call tools/compile.bat build.xml properties\jphone-gm.properties

rem
rem copy jad/jar files to authoring tool directory.
rem
call ant copy -Dsrcdir=..\spicydogj3\sdp -Ddstdir=..\output\release\sdp
call ant copy -Dsrcdir=..\spicydogj3\sdp -Ddstdir=..\output\debug\sdp
