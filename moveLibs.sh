#Moves Folders from Modules/Libraries to Arduino Library location
cd Modules/Libraries
cp -r DSerial/ KTANECommon/ LedControl/ NeoICSerial/ MAX6954/ $HOME/Documents/Arduino/libraries