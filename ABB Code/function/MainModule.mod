MODULE MainModule
	PROC main()
		testfunc;
	ENDPROC
	PROC testfunc()
		Reset doGripOpen;
		WaitTime 2;
		Set doGripClose;
		WaitTime 3;
		Reset doGripClose;
		WaitTime 2;
		Set doGripOpen;
		WaitTime 3;
		testfunc;
	ENDPROC
ENDMODULE