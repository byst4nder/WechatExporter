//
//  Exporter.h
//  WechatExporter
//
//  Created by Matthew on 2020/9/30.
//  Copyright © 2020 Matthew. All rights reserved.
//

#include <stdio.h>
#include <string>
#include <atomic>
#include <thread>

#include "Logger.h"
#include "Shell.h"
#include "WechatObjects.h"
#include "ITunesParser.h"
#include "DownloadPool.h"
#include "semaphore.h"
#include "ExportNotifier.h"

#ifndef Exporter_h
#define Exporter_h

class SessionParser;

class Exporter
{
protected:
	std::atomic_bool m_running;
	std::thread m_thread;

	// semaphore& m_signal;
    std::string m_workDir;
    
    std::string m_backup;
    std::string m_output;
    Logger* m_logger;
    Shell* m_shell;
    
    ITunesDb *m_iTunesDb;
    
    std::map<std::string, std::string> m_templates;
	std::map<std::string, std::string> m_localeStrings;

	ExportNotifier* m_notifier;
    
public:
    Exporter(const std::string& workDir, const std::string& backup, const std::string& output, Shell* shell, Logger* logger);
    ~Exporter();

	void setNotifier(ExportNotifier *notifier);
    
    bool run();
	bool isRunning() const;
	void waitForComplition();
    
    
    
protected:
	bool runImpl();
	bool exportUser(Friend& user, DownloadPool& downloadPool);
	// bool exportSession(Friend& user, Friends& friends, const Session& session, const std::string& userBase, const std::string& outputBase, DownloadPool& downloadPool);
	bool exportSession(Friend& user, const SessionParser& sessionParser, const Session& session, const std::string& userBase, const std::string& outputBase);


    bool fillUser(Friend& user);
	bool fillSession(Session& session, const Friends& friends) const;
    bool loadTemplates();
	bool loadStrings();
    std::string getTemplate(const std::string& key) const;
	std::string getLocaleString(std::string key) const;
    
    
    void notifyStart();
    void notifyComplete(bool cancelled = false);
    void notifyProgress(double progress);
};

#endif /* Exporter_h */