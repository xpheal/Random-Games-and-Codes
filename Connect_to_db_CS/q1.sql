SELECT N.Nutr_Val
	FROM FOOD_DES F, NUT_DATA N
	WHERE F.NDB_No=N.NDB_No AND F.Long_Desc LIKE "%McDONALD'S%" AND F.Long_Desc LIKE "%Hamburger%" AND n.nutr_no = 205;