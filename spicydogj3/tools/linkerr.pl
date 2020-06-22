# OLM.OBJ : error LNK2001: ŠO•”¼ÝÎÞÙ ""public: bool __thiscall SDOMArchive::IsStoring(void)const " (?IsStoring@SDOMArchive@@QBE_NXZ)" ‚Í–¢‰ðŒˆ‚Å‚·
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
