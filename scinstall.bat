@echo off
sc create echoreloadedservice binPath= \\"$$system_path($${target.path}\\\\$${TARGET}.exe) --backend windows\\" start= demand displayname= \\"%{ProjectName} Service\\" || exit /B 1
sc description echoreloadedservice \\"The Echo Reloaded Service\\" || exit /B 1
