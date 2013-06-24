PopCap Sexy Framework Target for Monkey
=======================================

Original Framework: http://sourceforge.net/projects/popcapframework/

License: http://forum.fischeronline.de/

Important Notes
---------------

1. Everyone that uses this say BIG THANKS to Mark Sibly.  Because 80% of mojo.sexy.cpp is mojo.glfw.cpp.  Most of mojo is matrix transformation code.  If I have to rewrite that chance are it will not be compatible with other targets!  I have emailed him asking for permission to release and he has given me the go ahead.

2. Per Popcap license you have to say thanks to them in the credit.  For example in the special thanks of the credit say, "PopCap for their great framework".

3. Per Popcap license you have to include their license text file in your game.  That is the file "PopCap Framework License.txt"

Why should I use this target? 
-----------------------------

Here are some reasons: (Taken from forum.fischeronline.de FAQ)
* It allows you spend more time developing games and less time dealing with technical issues.
* It gives you access to fast rendering through modern 3D hardware, while still allowing your game to run on older computers without proper 3d support... an important segment of the audience for casual games.
* The Framework's reliability has already been tested in dozens of game titles distributed to tens of millions of customers.
* Using heavily refined and tested code will help reduce QA time and ensure compatibility on the widest range of products.

How to install the target
-------------------------

1. Download DirectX SDK December 2006: http://www.microsoft.com/en-au/download/details.aspx?id=10990 (448MB!)  
You can use other version, as long as Direct X 7 is still there.  That is, Direct Draw and Direct 3D 7.  Install the DirectX SDK into the directory C:\sdk\Microsoft DirectX SDK (December 2006).  IF NOT YOU HAVE TO CHANGE IT IN THE VISUAL C++ SOLUTION FILE!

2. Download the target from github (https://github.com/Ferddi/psf-monkey), and copy the directory over into your monkey directory.  Hopefully seeing the github you will realise where those directory should go.

3. In the directory monkey/src/transcc/ there is a batch file "c.bat".  Double click on this file.  It will do the following to your Monkey directory:
A) Rename transcc_winnt.exe to transcc_winnt_original.exe
B) Compile the new transcc_winnt.exe to be able to compile the sexy framework.

4. In the directory monkey/modules/mojo/, in the file app.monkey, you need to add:
TARGET="sexy" Or - into the line so it becomes:
\#If TARGET="sexy" Or TARGET="android" Or TARGET="flash" Or TARGET="glfw" Or TARGET="html5" Or TARGET="ios" Or TARGET="psm" Or TARGET="win8" Or TARGET="xna"
so that it Import native/mojo.sexy.cpp.  IF YOU DO NOT DO THIS STEP, YOU WILL GET 101 ERRORS and 19 WARNINGS! 

5. That's it and hopefully when you open up Ted "PopCap Sexy Framework" will appear as a target.  AND you can compile the bananas.

Support
-------

Contact me Ferdi on our forums for support.

http://www.monkeycoder.co.nz/Community/posts.php?topic=5439

Or email me, my email address is at.

http://www.monkeycoder.co.nz/Account/showuser.php?id=414

      