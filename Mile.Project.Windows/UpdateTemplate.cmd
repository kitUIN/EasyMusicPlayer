@rem 
@rem PROJECT:   Mouri Internal Library Essentials
@rem FILE:      UpdateTemplate.cmd
@rem PURPOSE:   Update template to project script
@rem 
@rem LICENSE:   The MIT License
@rem 
@rem DEVELOPER: Mouri_Naruto (Mouri_Naruto AT Outlook.com)
@rem 

@echo off

copy "%~dp0.editorconfig" "%~dp0..\"
copy "%~dp0.gitignore" "%~dp0..\"

echo Update completed.
