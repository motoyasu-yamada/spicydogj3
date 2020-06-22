rem
rem copy upload templates to upload.spicy-dog.com.
rem 
dir ..\output\server\
set PSCP_TO=m_yamada@nfs.find.spicysoft.jp:/home/sites/upload.spicy-dog.com/template/jlite3
set PSCP_FROM=-r ..\output\server\*
set PSCP_PPK=c:\doc\m_yamada.ppk
pscp -v -i %PSCP_PPK%  %PSCP_FROM%  %PSCP_TO%
