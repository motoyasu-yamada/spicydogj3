# OLM.OBJ : error LNK2001: 外部ｼﾝﾎﾞﾙ ""public: bool __thiscall SDOMArchive::IsStoring(void)const " (?IsStoring@SDOMArchive@@QBE_NXZ)" は未解決です
#
print "Pickup link error.";
while (<>) {
	if (/^.+\"\"(.+)\"\s*\(.+\)\s*\".+$/) {
		if ($errors{$1} ne "done") {
			print $1."\n";
			$errors{$1} = "done";
		}
	}
}
