
/*
	Workaround for link errors on MacOS with UE 4.24

		[2/8] Link UE4Editor-V8-Mac-DebugGame.dylib
		Undefined symbols for architecture x86_64:
			"vtable for FFrame", referenced from:
				FFrame::FFrame(UObject*, UFunction*, void*, FFrame*, UField*) in Module.V8.cpp.o
				FFrame::~FFrame() in Module.V8.cpp.o
			NOTE: a missing vtable usually means the first non-inline virtual member function has no definition.

	According to Stack.h, FFrame only has two virtuals: its destructor (implemented in the header),
	and Serialize(). Theoretically Serialize() is implemented in ScriptCore.cpp, but somehow
	it doesn't make it to the binary in my case.
	As a temporary workaround, FFrame::Serialize() is duplicated here,
	this seems to fix the error.
*/

DEFINE_LOG_CATEGORY_STATIC(LogScriptCore, Log, All);


void FFrame::Serialize( const TCHAR* V, ELogVerbosity::Type Verbosity, const class FName& Category )
{
	// Treat errors/warnings as bad
	if (Verbosity == ELogVerbosity::Warning)
	{
#if !UE_BUILD_SHIPPING
		static bool GTreatScriptWarningsFatal = FParse::Param(FCommandLine::Get(),TEXT("FATALSCRIPTWARNINGS"));
		if (GTreatScriptWarningsFatal)
		{
			Verbosity = ELogVerbosity::Error;
		}
#endif
	}
	if( Verbosity==ELogVerbosity::Error )
	{
		UE_LOG(LogScriptCore, Fatal,
			TEXT("%s\r\n\t%s\r\n\t%s:%04X\r\n\t%s"),
			V,
			*Object->GetFullName(),
			*Node->GetFullName(),
			Code - Node->Script.GetData(),
			*GetStackTrace()
		);
	}
	else
	{
#if DO_BLUEPRINT_GUARD
		UE_LOG(LogScript, Warning,
			TEXT("%s\r\n\t%s\r\n\t%s:%04X%s"),
			V,
			*Object->GetFullName(),
			*Node->GetFullName(),
			Code - Node->Script.GetData(),
//			ShowKismetScriptStackOnWarnings()
			true
				? *(FString(TEXT("\r\n")) + GetStackTrace())
				: TEXT("")
		);
#endif
	}
}
