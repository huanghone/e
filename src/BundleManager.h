/*******************************************************************************
 *
 * Copyright (C) 2009, Alexander Stigsen, e-texteditor.com
 *
 * This software is licensed under the Open Company License as described
 * in the file license.txt, which you should have received as part of this
 * distribution. The terms are also available at http://opencompany.org/license.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ******************************************************************************/

#ifndef __BUNDLEMANAGER_H__
#define __BUNDLEMANAGER_H__

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <vector>
#include "FileInfo.h"
#include "BundleInfo.h"


class ITmLoadBundles;
class PListHandler;
class wxFileName;
class wxProgressDialog;
class wxListCtrl;
class wxListEvent;
class IHtmlWnd;
class IHtmlWndBeforeLoadEvent;
class RemoteThread;
class cxRemoteListEvent;
class cxRemoteAction;


class BundleManager : public wxDialog {
public:
	BundleManager(wxWindow *parent, RemoteThread& remoteThread, ITmLoadBundles* syntaxHandler);
	~BundleManager();

	bool NeedBundleReload() const {return m_needBundleReload;};

private:
	void LoadBundleLists();
	void UpdateBundleList();
	void RestoreBundle();
	bool InstallBundle();
	bool InstallBundle(const cxFileInfo* bundle, const wxString& url, long ix);
	bool DownloadDir(const wxString& url, const wxFileName& path, wxProgressDialog& dlg);

	void AddItems(const wxString& repoName, const std::vector<cxFileInfo>& bundleList);
	void SetDirModDate(wxFileName& path, const wxDateTime& modDate);
	void DelTree(const wxString& path);

	void SelectItem(long itemId, bool update=false);

	const wxString& GetCurrentRepoUrl() const;
	const wxString& GetRepoUrl(const wxString& name) const;

	// Event handlers
	void OnRemoteListReceived(cxRemoteListEvent& event);
	void OnRemoteAction(cxRemoteAction& event);
	void OnItemSelected(wxListEvent& event);
	void OnInstallButton(wxCommandEvent& event);
	void OnDeleteButton(wxCommandEvent& event);
	void OnClose(wxCloseEvent& event);
	void OnBeforeLoad(IHtmlWndBeforeLoadEvent& event);
	void OnUpdateAllBundles(wxCommandEvent& event);

	DECLARE_EVENT_TABLE()

	enum BundleState {
		BDL_NOT_INSTALLED,
		BDL_INSTALLED_UPTODATE,
		BDL_INSTALLED_OLDER,
		BDL_INSTALLED_NEWER,
		BDL_DISABLED
	};
	
	cxBundleInfo* GetState(const cxFileInfo* bundle, BundleState& state);

	class RepoInfo {
	public:
		RepoInfo(const wxString& name, const wxString& url) : name(name), url(url), received(false) {};
		RepoInfo& operator=(const RepoInfo& ri) {
			name = ri.name;
			url = ri.url;
			received = ri.received;
			bundleList = ri.bundleList;
			return *this;
		};
		wxString name;
		wxString url;
		bool received;
		std::vector<cxFileInfo> bundleList;
	};

	// Member variables
	RemoteThread& m_remoteThread;
	ITmLoadBundles* m_syntaxHandler;
	PListHandler& m_plistHandler;
	wxString m_tempFile;
	std::vector<cxBundleInfo> m_installedBundles;
	std::vector<RepoInfo> m_repositories;
	bool m_allBundlesReceived;
	bool m_needBundleReload;
	static const wxString s_emptyString; // to avoid return temp warning

	// Current selection
	long m_currentSel;
	wxString m_currentRepo;
	const cxFileInfo* m_currentBundle;
	cxBundleInfo* m_currentBundleInfo;
	BundleState m_currentBundleState;

	// Member ctrls
	wxSizer* m_mainSizer;
	wxListCtrl* m_bundleList;
	IHtmlWnd* m_browser;
	wxStaticText* m_statusText;
	wxButton* m_installButton;
	wxButton* m_deleteButton;
};

#endif // __BUNDLEMANAGER_H__
