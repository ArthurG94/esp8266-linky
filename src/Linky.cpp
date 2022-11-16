
#include "Linky.h"

Linky::Linky() : serial(13)
{
}

void Linky::begin()
{
	memset(this->data, 0, sizeof(this->data));
	this->serial.begin(1200, SWSERIAL_7E1);
}

/*
Structure d'une ligne 
LF (0x0A)
ETIQUETTE (4 à 8 caractères)
SP (0x20)
DATA (1 à 12 caractères)
SP (0x20)
CHECKSUM (caractère de contrôle : Somme de caractère)
CR (0x0D)
*/

void Linky::loop()
{
	while (this->serial.available() > 0)
	{
		char c = this->serial.read();

		//On ignore les caractères de debut et fin de transmission
		if (c != 0x02 && c != 0x03)
		{
			this->data[this->dataLength] = c;

			if (this->data[this->dataLength] == 0x0D)
			{

				String key = "";
				String value = "";
				int mode = 0;
				for (char i = 1; i < this->dataLength; i++)
				{
					char c = this->data[i];

					if (c == 0x20)
					{
						mode++;
					}
					else
					{
						if (mode == 0)
						{
							key += c;
						}
						if (mode == 1)
						{
							value += c;
						}
					}
				}

				if (key == "ADCO")
				{
					this->ADCO = value;
				}
				if (key == "OPTARIF")
				{
					this->OPTARIF = value;
				}
				if (key == "ISOUSC")
				{
					this->ISOUSC = value.toInt();
				}
				if (key == "BASE")
				{
					this->BASE = value;
				}
				if (key == "PTEC")
				{
					this->PTEC = value;
				}
				if (key == "IINST")
				{
					this->IINST = value.toInt();
				}
				if (key == "IMAX")
				{
					this->IMAX = value.toInt();
				}
				if (key == "PAPP")
				{
					this->PAPP = value.toInt();
				}
				if (key == "HHPHC")
				{
					this->HHPHC = value;
				}
				if (key == "MOTDETAT")
				{
					this->MOTDETAT = value;
				}

				memset(this->data, 0, sizeof(this->data));
				this->dataLength = 0;
			}
			else
			{
				this->dataLength++;
			}
		}
	}
}
