REM
REM Use gzip to compress static files
REM
call gzip    Tool html Styles                      css
call gzip    Tool html events                      js
call gzip    Tool html jquery                      js
call gzip    Tool html RGraphCC                    js
call gzip    Tool html RGraphCE                    js
call gzip    Tool html RGraphLi                    js
REM
REM Convert the compressed files
REM Copy the .c/.h files into "Generated" folder
REM Delete the temp files from HTML folder
REM
call cc_gzip Tool html  Styles                     gz
call cc_gzip Tool html  events                     gz
call cc_gzip Tool html  jquery                     gz
call cc_gzip Tool html  RGraphCC                   gz
call cc_gzip Tool html  RGraphCE                   gz
call cc_gzip Tool html  RGraphLi                   gz
REM
REM Convert regular files
REM
call cc      Tool html Empty                       gif
call cc      Tool html GreenRUp                    gif
call cc      Tool html Logo                        gif
call cc      Tool html RedRDown                    gif
call cc      Tool html WhiteR                      gif
call cc      Tool html About                       htm --compress
call cc      Tool html Authen                      htm --compress
call cc      Tool html Error404                    htm --compress
call cc      Tool html FormGET                     htm --compress
call cc      Tool html FormPOST                    htm --compress
call cc      Tool html index                       htm --compress
call cc      Tool html IPConf                      htm --compress
call cc      Tool html IPInf                       htm --compress
call cc      Tool html OSInf                       htm --compress
call cc      Tool html Products                    htm --compress
call cc      Tool html SendMail                    htm --compress
call cc      Tool html Shares                      htm --compress
call cc      Tool html SSE_IP                      htm --compress
call cc      Tool html SSE_OS                      htm --compress
call cc      Tool html SSE_Time                    htm --compress
call cc      Tool html Upl                         htm --compress
call cc      Tool html Upl_AJAX                    htm --compress
call cc      Tool html VirtFile                    htm --compress
call cc      Tool html favicon                     ico
call cc      Tool html BTL_Det                     jpg
call cc      Tool html BTL_Pic                     jpg
call cc      Tool html FS_Det                      jpg
call cc      Tool html FS_Pic                      jpg
call cc      Tool html GUI_Det                     jpg
call cc      Tool html GUI_Pic                     jpg
call cc      Tool html IP_Det                      jpg
call cc      Tool html IP_Pic                      jpg
call cc      Tool html OS_Det                      jpg
call cc      Tool html OS_Pic                      jpg
call cc      Tool html USBD_Det                    jpg
call cc      Tool html USBD_Pic                    jpg
call cc      Tool html USBH_Det                    jpg
call cc      Tool html USBH_Pic                    jpg
call cc      Tool html BGround                     png

@echo off
echo Batch files will become obsoleted by the end of 2025, please use GenFiles.py
pause