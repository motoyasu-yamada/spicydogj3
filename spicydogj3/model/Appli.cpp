#include "stdafx.h"
#include "Appli.h"
#include "resource.h"

#include "sdom/sdomn.h"
#include "AppliConf.h"
#include "char/CharList.h"
#include "stage/MainStage.h"
#include "stage/SubStages.h"
#include "build/Builder.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

Appli::Appli() {
	appliConf = APPLICONFP(new AppliConf);
	charList  = CHARLISTP (new CharList);
	mainStage.OwnNew(new MainStage);
	subStages     = SUBSTAGESP(new SubStages);
	m_activeStage = NULL;
}

Appli::~Appli() {

}

void Appli::AssertValid() const {
	appliConf->AssertValid();
	charList->AssertValid();
	mainStage->AssertValid();
	subStages->AssertValid();
}

SDOMSerializable* factoryMainStage(LPCWSTR name) {
	if (name != WSDOMN_MAIN_STAGE) {
	}
	return new MainStage;
}

LPCWSTR namerMainStage(SDOMSerializable* object) {
	ASSERT(AfxIsValidAddress(object,sizeof MainStage));
	return WSDOMN_MAIN_STAGE;
}

void Appli::Serialize(SDOMArchive* ar) {
	if (ar->IsLoading()) {
		m_activeStage = NULL;
	}
	ar->SerializeStatic(WSDOMN_APPLI_CONF,appliConf.get());
	ar->SerializeStatic(WSDOMN_CHAR_LIST, charList.get());
	ar->SerializeOwner (&mainStage,true,factoryMainStage,namerMainStage);
	ar->SerializeStatic(WSDOMN_SUB_STAGES,subStages.get());
}

void Appli::Build(Builder* builder) const {
	builder->Build(appliConf.get());
	builder->Build(charList.get());

	builder->BeginInstruction();
		builder->Build(mainStage);
		builder->Build(subStages.get());
		if (builder->GetBuildStep() == Builder::BUILDSTEP_LAST) {
			builder->WriteInstruction(INSTID_END);
		}
	builder->EndInstruction();
}

void Appli::ActivateStage(const Stage* stage) {
	ASSERT_VALID(m_activeStage);
	ASSERT_VALID(stage);
	if (m_activeStage == stage) {
		return;
	}

	m_activeStage = const_cast<Stage*>(stage);
	ASSERT((MainStage*)mainStage == stage || subStages->Exists(stage));
}

Stage* Appli::GetActiveStage() {
	if (m_activeStage == NULL) {
		m_activeStage = mainStage;
	}
	ASSERT((MainStage*)mainStage == m_activeStage || subStages->Exists(m_activeStage));
	return m_activeStage;
}

void Appli::GetLabelList(LABEL_NAME_LIST& list) const {
	mainStage->GetLabelList(list);
	subStages->GetLabelList(list);
}
