SELECT n.nutr_no, nd.nutrdesc, count(*)
	FROM nut_data n, nutr_def nd
	WHERE n.nutr_no = nd.nutr_no
	GROUP BY n.nutr_no;
