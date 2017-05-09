SELECT f.ndb_no as ndb_no, f.fdgrp_cd, f.shrt_desc
	FROM food_des f, nut_data n, nutr_def nd
	WHERE f.ndb_no=n.ndb_no AND n.nutr_no = nd.nutr_no AND nd.nutrdesc = "Water"
EXCEPT
SELECT f.ndb_no as ndb_no, f.fdgrp_cd, f.shrt_desc
	FROM food_des f, nut_data n, nutr_def nd
	WHERE f.ndb_no=n.ndb_no AND n.nutr_no = nd.nutr_no AND nd.nutrdesc = "Calcium, Ca"
ORDER BY f.fdgrp_cd DESC, f.shrt_desc ASC;
