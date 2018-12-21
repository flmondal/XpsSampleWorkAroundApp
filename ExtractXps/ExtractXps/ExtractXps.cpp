#include "stdafx.h"
#include "XPSDocumentHelper.h"
#include "Decryptor.h"

using namespace System;
using namespace System::IO;


//using namespace Ipc;

array<Byte> ^getCurrentContentBytes(String ^fileName)
{

	array<Byte> ^bytesRead =File::ReadAllBytes(fileName);
	return bytesRead;

}


System::Void ConvertXPSTOTextFile(String ^fileName)
{
	FileStream  ^saveFile;
	array<Byte> ^currentBytes;

	saveFile = nullptr;

	try
	{
		// Get content of the file to a byte array
		currentBytes = getCurrentContentBytes(fileName);

		// get exclusive access to the file
		FileStream  ^savedBinaryTextFile;
		savedBinaryTextFile = gcnew FileStream("C:\\Users\\flmondal\\Desktop\\AIP\\ExtractXPSDocument\\Docs\\protected\\Temp_TextFile_From_XpsDocumentNOTPROTECTED.txt", FileMode::OpenOrCreate, FileAccess::ReadWrite, FileShare::None);

		savedBinaryTextFile->Write(currentBytes, 0, currentBytes->Length);

		savedBinaryTextFile->Close();

	}
	catch (Exception ^e)
	{
		System::Console::WriteLine(e->Message);
	}
	finally
	{
		currentBytes = nullptr;

		// flush & close
		if (saveFile != nullptr)
		{
			saveFile->Flush();
			saveFile->Close();
			saveFile = nullptr;
		}
		
	}

	System::Console::WriteLine("Hello From ConvertXPSTOTextFile");
}

System::Void ConvertTextFileToXPS(String ^fileName)
{
	FileStream  ^saveFile;
	array<Byte> ^currentBytes;

	saveFile = nullptr;
	try
	{
		// Get content of the file to a byte array
		currentBytes = getCurrentContentBytes(fileName);

		// get exclusive access to the file
		FileStream  ^savedBinaryXPSFile;
		savedBinaryXPSFile = gcnew FileStream("C:\\Users\\flmondal\\Desktop\\AIP\\ExtractXPSDocument\\Docs\\protected\\NewTestXPSDocumentNOTPROTECTED.xps", FileMode::OpenOrCreate, FileAccess::ReadWrite, FileShare::None);
		savedBinaryXPSFile->Write(currentBytes, 0, currentBytes->Length);

		savedBinaryXPSFile->Close();
	}
	catch (Exception ^e)
	{
		System::Console::WriteLine(e->Message);
	}
	finally
	{
		currentBytes = nullptr;

		// flush & close
		if (saveFile != nullptr)
		{
			saveFile->Flush();
			saveFile->Close();
			saveFile = nullptr;
		}

	}
}


array<Byte> ^returnBytesFromTheFileAsBinaryData(String ^fileName)
{
	FileStream  ^saveFile;
	array<Byte> ^currentBytes;

	saveFile = nullptr;

	try 
	{		
		//Get content of the file to a byte array
		currentBytes = getCurrentContentBytes(fileName);

		FileStream  ^savedBinaryFile;
		savedBinaryFile = gcnew FileStream("C:\\Users\\flmondal\\Desktop\\AIP\\ExtractXPSDocument\\Docs\\protected\\Binary_From_XpsDocumentPROTECTED.txt", FileMode::OpenOrCreate, FileAccess::ReadWrite, FileShare::None);
		
		savedBinaryFile->Write(currentBytes, 0, currentBytes->Length);
	}
	catch(Exception ^e)
	{
		System::Console::WriteLine(e->Message);
	}
	finally
	{
		// flush & close
		if (saveFile != nullptr)
		{
			saveFile->Flush();
			saveFile->Close();
			saveFile = nullptr;
		}
	}
	return currentBytes;
}



int main(array<System::String ^> ^args)
{
	/*ConvertXPSTOTextFile("C:\\Users\\flmondal\\Desktop\\AIP\\ExtractXPSDocument\\Docs\\TestXPSDocument.xps");
	ConvertTextFileToXPS("C:\\Users\\flmondal\\Desktop\\AIP\\ExtractXPSDocument\\Docs\\Temp_TextFile_From_XpsDocument.txt");
	*/
	/*
	ConvertXPSTOTextFile("C:\\Users\\flmondal\\Desktop\\AIP\\ExtractXPSDocument\\Docs\\protected\\TestXPSDocumentNOTProtected.xps");
	ConvertTextFileToXPS("C:\\Users\\flmondal\\Desktop\\AIP\\ExtractXPSDocument\\Docs\\protected\\Temp_TextFile_From_XpsDocumentNOTPROTECTED.txt");
	*/
	

	String ^fileDirPath = "C:\\Users\\flmondal\\Desktop\\AIP\\ExtractXPSDocument\\Docs\\protected\\";

	array<Byte> ^contentBytesBeforDecryption, ^contentBytesAfterDecryption;

	//STEP 1: Read the contents of the protected xps file as a byte array
	contentBytesBeforDecryption = returnBytesFromTheFileAsBinaryData(fileDirPath + "TestXPSDocumentProtected.xps");


	
	//STEP 2: Decrypt the contents 
	Decryptor    ^decryptor;
	MemoryStream    ^temporaryStream;
	decryptor = gcnew Decryptor();
	temporaryStream = decryptor->Decrypt(contentBytesBeforDecryption);

	//STEP 3: Copy contents of the file after Decryption to a array of bytes
	contentBytesAfterDecryption = gcnew array<Byte>((int)temporaryStream->Length);
	temporaryStream->Read(contentBytesAfterDecryption, 0, (int)temporaryStream->Length);
	temporaryStream->Close();
	temporaryStream = nullptr;

	//STEP 4: Save the contents of the decrypted file from the memory stream to a txt file
	FileStream  ^savedBinaryFileAfterDecryption;
	savedBinaryFileAfterDecryption = gcnew FileStream(fileDirPath + "DecryptedXpsDocumentPROTECTEDFile.txt", FileMode::OpenOrCreate, FileAccess::ReadWrite, FileShare::None);
	savedBinaryFileAfterDecryption->Write(contentBytesAfterDecryption, 0, contentBytesAfterDecryption->Length);

	//STEP 5: Convert the text file back to Xps file.
	ConvertTextFileToXPS(fileDirPath + "DecryptedXpsDocumentPROTECTEDFile.txt");

	return 0;


}
