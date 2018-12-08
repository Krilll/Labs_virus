del "virus.txt.exe"
for %%i in (*.txt.exe) do ( 
more < %%i:secret.txt > %%i  
ren "%%i" *.
)
