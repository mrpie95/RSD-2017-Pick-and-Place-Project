MODULE MainModule
	PROC main()
		Reset doGripClose;
		Set doGripOpen;
		WaitTime 2;
		Reset doGripOpen;
	ENDPROC
ENDMODULE