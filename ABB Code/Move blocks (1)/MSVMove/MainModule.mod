MODULE MainModule
	PROC main()
		Reset doGripClose;
		Set doGripOpen;
		WaitTime 2;
		Reset doGripOpen;
        
		MoveJ [[311.11,255.74,192.28],[0.0171914,0.712546,0.700954,0.0254322],[0,-1,-3,0],[9E+09,9E+09,9E+09,9E+09,9E+09,9E+09]], v1000, z50, tool0;
		WaitTime 3;
		Reset doGripOpen;
		Set doGripClose;
		WaitTime 2;
		Reset doGripClose;
		WaitTime 1;
		MoveJ [[311.11,255.74,584.82],[0.017195,0.712577,0.700922,0.0254051],[0,-1,-3,0],[9E+09,9E+09,9E+09,9E+09,9E+09,9E+09]], v1000, z50, tool0;
		WaitTime 2;
		MoveJ [[348.27,-57.16,189.24],[0.0171718,0.712621,0.700879,0.0253798],[-1,0,-4,0],[9E+09,9E+09,9E+09,9E+09,9E+09,9E+09]], v1000, z50, tool0;
		WaitTime 2;
		Reset doGripClose;
		Set doGripOpen;
		WaitTime 2;
		Reset doGripOpen;
		MoveJ [[348.47,4.18,520.14],[0.01718,0.712616,0.700884,0.0253828],[0,0,-3,0],[9E+09,9E+09,9E+09,9E+09,9E+09,9E+09]], v1000, z50, tool0;
		WaitTime 2;
		MoveJ [[348.27,-57.16,189.24],[0.0171718,0.712621,0.700879,0.0253798],[-1,0,-4,0],[9E+09,9E+09,9E+09,9E+09,9E+09,9E+09]], v1000, z50, tool0;
		WaitTime 2;
		Reset doGripOpen;
		Set doGripClose;
		WaitTime 2;
		Reset doGripClose;
		MoveJ [[348.27,-57.16,189.24],[0.0171718,0.712621,0.700879,0.0253798],[-1,0,-4,0],[9E+09,9E+09,9E+09,9E+09,9E+09,9E+09]], v1000, z50, tool0;
		MoveJ [[311.11,255.74,584.82],[0.017195,0.712577,0.700922,0.0254051],[0,-1,-3,0],[9E+09,9E+09,9E+09,9E+09,9E+09,9E+09]], v1000, z50, tool0;
		MoveJ [[311.11,255.74,192.28],[0.0171914,0.712546,0.700954,0.0254322],[0,-1,-3,0],[9E+09,9E+09,9E+09,9E+09,9E+09,9E+09]], v1000, z50, tool0;
		WaitTime 2;
		Reset doGripClose;
		Set doGripOpen;
		WaitTime 2;
		Reset doGripOpen;
		MoveJ [[348.47,4.18,520.14],[0.01718,0.712616,0.700884,0.0253828],[0,0,-3,0],[9E+09,9E+09,9E+09,9E+09,9E+09,9E+09]], v1000, z50, tool0;
	ENDPROC
ENDMODULE