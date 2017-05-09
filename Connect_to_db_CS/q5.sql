SELECT F.Long_Desc, avg(N.Nutr_Val), m.avg_val
FROM
	(
		SELECT F.NDB_No, avg(N.Nutr_Val) as avg_val
			FROM FOOD_DES F, NUT_DATA N
			WHERE F.NDB_No=N.NDB_No AND F.Long_Desc LIKE "%McDONALD'S%" AND F.Long_Desc LIKE "%Hamburger%"
			GROUP BY F.NDB_No
	) m,
	FOOD_DES F,
	NUT_DATA N
WHERE F.NDB_No = N.NDB_No
GROUP BY F.NDB_NO
HAVING avg(N.Nutr_Val) > m.avg_val;