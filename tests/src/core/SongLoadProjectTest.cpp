/*
 * SongLoadProjectTest.cpp
 *
 * Copyright (c) 2026 LMMS Developers
 *
 * This file is part of LMMS - https://lmms.io
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with LMMS; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

#include <QTemporaryDir>
#include <QtTest>

#include "Engine.h"
#include "Song.h"

class SongLoadProjectTest : public QObject
{
	Q_OBJECT
private slots:
	void initTestCase()
	{
		lmms::Engine::init(true);
	}

	void cleanupTestCase()
	{
		lmms::Engine::destroy();
	}

	void failedLoadRestoresProjectState()
	{
		using namespace lmms;

		QTemporaryDir tempDir;
		QVERIFY(tempDir.isValid());

		auto song = Engine::getSong();
		const QString projectFileName = tempDir.filePath("project.mmp");
		QVERIFY(song->saveProjectFile(projectFileName));
		QCOMPARE(song->loadProject(projectFileName), Song::ProjectLoadStatus::Success);
		QCOMPARE(song->projectFileName(), projectFileName);

		const QString missingFileName = tempDir.filePath("missing.mmp");
		QCOMPARE(song->loadProject(missingFileName), Song::ProjectLoadStatus::Failed);
		QCOMPARE(song->projectFileName(), projectFileName);
		QVERIFY(!song->isLoadingProject());
	}
};

QTEST_GUILESS_MAIN(SongLoadProjectTest)
#include "SongLoadProjectTest.moc"
