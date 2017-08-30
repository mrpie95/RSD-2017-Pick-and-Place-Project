MODULE MainModule
	PROC main()
		Reset doGripClose;
		Set doGripOpen;
		WaitTime 2;
		Reset doGripOpen;
        WaitTime 4;
        Reset doGripOpen;
		Set doGripClose;
		WaitTime 2;
		Reset doGripClose;
        MoveJ [[311.11,255.74,192.28],[0.0171914,0.712546,0.700954,0.0254322],[0,-1,-3,0],[9E+09,9E+09,9E+09,9E+09,9E+09,9E+09]], v1000, z50, tool0;
	ENDPROC
ENDMODULE