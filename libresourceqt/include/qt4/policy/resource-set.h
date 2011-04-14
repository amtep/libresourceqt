/*************************************************************************
This file is part of libresourceqt

Copyright (C) 2010 Nokia Corporation.

This library is free software; you can redistribute
it and/or modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation
version 2.1 of the License.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301
USA.
*************************************************************************/
/**
* \file resource-set.h
* \brief Declaration of ResourcePolicy::ResourceSet
*
* \copyright Copyright (C) 2010 Nokia Corporation.
* \author Wolf Bergenheim and Robert Löfman
* \par License
* @license LGPL
* This file is part of libresourceqt
* \par
* Copyright (C) 2010 Nokia Corporation.
* \par
* This library is free software; you can redistribute
* it and/or modify it under the terms of the GNU Lesser General Public
* License as published by the Free Software Foundation
* version 2.1 of the License.
* \par
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
* Lesser General Public License for more details.
* \par
* You should have received a copy of the GNU Lesser General Public
* License along with this library; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301
* USA.
*/

#ifndef RESOURCE_SET_H
#define RESOURCE_SET_H

#include <QString>
#include <QObject>
#include <QVector>
#include <QList>
#include <policy/resources.h>
#include <policy/audio-resource.h>

/**
* \mainpage The Resource Policy API: Libresourceqt
*
* \section intro_section Introduction
*
* This library is used to request resources from the Policy Resource Manager.
* To use this library two classes are provided: \ref ResourcePolicy::Resource and
* \ref ResourcePolicy::ResourceSet.
*
* The Libresourceqt API provides:
*
* <table border="1">
* <caption>
*   <big><b>
*     Classes in Libresourceqt API
*   </b></big>
* </caption>
* <tr>
*   <th>Name</th>
*   <th>Content</th>
* </tr>
* <tr>
*   <td>ResourcePolicy::ResourceSet</td>
*   <td>The \ref ResourceSet is the main class whose instances should be filled with needed resources. Use the ResourceSet's methods to acquire those resources. </td>
* <tr>
*   <td>ResourcePolicy::Resource</td>
*   <td>The ResourceSet is filled with instances of subclasses of the class \ref Resource, for example </td>
* </tr>
* </table>
*
* \section library_started_section Getting started
*
* To use the Resource Policy Library, you first need to create the
* \ref ResourcePolicy::ResourceSet like this:
* \code
* ResourcePolicy::ResourceSet*resources = new ResourcePolicy::ResourceSet("player");
* \endcode
* Then to add resources to the set use the ResourceSet::addResource() method to add
* resources to the ResourcePolicy::ResourceSet. Like this:
* \code
* resources->addResource(AudioPlaybackType);
* resources->addResource(VideoPlaybackType);
* \endcode
* If you want to pre-populate the AudioResource with the audio group (it is a good idea)
* and other audio parameters you can create an audio object yourself and then
* give that to the ResourcePolicy::ResourceSet. Note that you should NOT free this object.
* The ResourcePolicy::ResourceSet takes ownership of this pointer.
* \code
* ResourcePolicy::AudioResource*audioResource = new ResourcePolicy::AudioResource("player");
* resources->addResourceObject(audioResource);
* \endcode
* Calling the ResourcePolicy::ResourceSet::deleteResource() method will remove
* and delete the object. Then when you want to acquire the ResourcePolicy::ResourceSet
* you simply use the ResourcePolicy::ResourceSet::acquire() method, like this:
* \code
* QObject::connect(resources, SIGNAL(resourcesAcquired()), this, SLOT(acquireOkHandler(QList<ResourcePolicy::Resource>)));
* QObject::connect(resources, SIGNAL(resourcesDenied()), this, SLOT(acquireDeniedHandler()));
* resources->acquire();
* \endcode
* You should also connect to the ResourcePolicy::ResourceSet::lostResources() signal like this:
* \code
* QObject::connect(resources, SIGNAL(lostResources()), this, SLOT(lostResources()));
* \endcode
* This signal tells you when you should stop using the resources you have asked for.
* So it is important that you connect to it.
*
* To modify the properties of the resources you can use the ResourcePolicy::ResourceSet::resource() method.
*
* \section see_further_info Further information
* For a more detailed guide see the
* <a href="http://wiki.meego.com/images/Meego-policy-framework-developer-guide.pdf">MeeGo Policy Framework Developer Guide (PDF)</a>.
*/

/**
* \brief The Namespace for Resource Policy.
* All Resource Policy Qt APIs reside under the ResourcePolicy namespace.
*/
namespace ResourcePolicy
{

enum requestType { Acquire=0, Update, Release } ;

class ResourceEngine;
/**
* Needed resources must be added to the ResourceSet. Each set can only contain
* a single Resource of a given type. That is one AudioPlaybackResource, etc.
*
* Internally the set is stored as a QVector of \ref Resource objects.
*/

class ResourceSet: public QObject
{
	Q_OBJECT
	Q_DISABLE_COPY(ResourceSet)
public:

	/**
	* Alternative constructor with additional parameters for setting
	* alwaysReply and autoRelease.
	* \param applicationClass This parameter defines the application class.
	* The application class is used to determine the priority order of the
	* application.
	* \param parent The parent of this class.
	* \param alwaysReply see setAlwaysReply()
	* \param autoRelease see setAutoRelease()
	*/
	ResourceSet(const QString &applicationClass, QObject*parent, bool alwaysReply, bool autoRelease);

	/**
	* Alternative backwards-compatible constructor.
	* \param applicationClass This parameter defines the application class.
	* The application class is used to determine the priority order of the
	* application.
	* \param parent The optional parent of of this class.
	*/
	ResourceSet(const QString &applicationClass, QObject*parent = NULL);

	/**
	* The destructor
	*/
	~ResourceSet();

	/**
	* This method adds a resource to the set. A set contains only a single
	* instance of a given resource. If the ResourceSet already contains a
        * resource of the given type it will be written over.
	* \param resourceType The resource to add to the set. A copy of this object
        * is stored in the ResourceSet.
	*/
	bool addResource(ResourceType resourceType);

	/**
	* This method adds a resource to the set. A set contains only a single
	* instance of a given resource. If the ResourceSet already contains a
        * resource of the given type it will be written over.
	* \param resource The resource to add to the set.
	* The ResourcePolicy::ResourseSet takes ownership of the pointer. Do NOT free!
	*/
	void addResourceObject(Resource*resource);

	/**
	* This method removes the resource of the given type
	* \param type The type of the resource to remove from the set.
	*/
	void deleteResource(ResourceType type);

	/**
	* This method returns a list of all resource in the set.
	* \return a QList of all resources in the set.
	*/
	QList<Resource*> resources() const;

	/**
	* This method returns a const pointer to a resource of a specific type.
	* \param type The type of resource we are interested in.
	* \return a pointer to the Resource if it is defined NULL otherwise.
	*/
	Resource* resource(ResourceType type) const;

	/**
	* Checks if the \ref ResourceSet contains the given \ref Resource
	* \param type The Resource to look for
	* \return true if the \ref Resource is defined in this \ref ResourceSet
	*/
	bool contains(ResourceType type) const;

	/**
	 * Checks if the \ref ResourceEngine of this set is connected to the manager.
	 * @return true if \ref resourceEngine is connected to the manager.
	 */
	bool isConnectedToManager() const;

	/**
	* Checks if the \ref ResourceSet contains all given resources.
	* \param types A list of resources to check for
	* \return true if \b all given resources are defined in the ResourceSet.
	*/
	bool contains(const QList<ResourceType> &types) const;

	/**
	* Returns the unique identifier for this ResourceSet.
	* @return the unique identifier for this ResourceSet.
	*/
	quint32 id() const;

	/**
	* Returns the registered application class (given in the constructor).
	*/
	QString applicationClass();

	/**
	 * Initializes and connects the ResourceEngine of this ResourceSet.
	 * This method is used after adding resources to the ResourceSet initially.
	 * \return true if the method succeeds without encountering errors.
	 */
	bool initAndConnect();

	/**
	* Try to acquire the \ref ResourceSet. The resourcesGranted() or
	* resourcesDenied() signal will be emitted depending on whether the
	* requested resources could be acquired or not.
	*/
	bool acquire();

	/**
	* Releases the acquired resources.
	*/
	bool release();

	/**
	* Commits changes to the \ref ResourcePolicy::ResourceSet. Remember to call update()
        * after adding and/or removing resources. Note that, if you have no resources aquired when
        * calling update() then this method only informs the policy manager of which resources you are
        * intersted in, and this request is acknowledged with a \ref updateOK() signal (if alwasReply is on).
        * If you do have resources granted then the application will be acknowledged with a \ref resourcesGranted()
        * or \ref lostResources() signal.
	*/
	bool update();

	/**
	* Sets the auto-release. When loosing the resources due to another
	* application with a higher priority the default is that we automatically
	* re-gain our resources without having to re-request them. However if
	* the AutoRelease is set we release the resources and need to re-acquire
	* them, when the pre-emting application releases its resources.
	*
	* This feature is by default disabled.
	*
	* This flag should be set once only before calling anything else
	* (excluding setAlwaysReply()), and cannot be unset.
	*/
	bool setAutoRelease();

	/**
	* Checks whether we have setAutoRelease().
	* \return true if auto-release is ennabled.
	*/
	bool willAutoRelease();

	/**
        * Assures that the resourcesGranted() signal are emitted even if we already
        * have the requested resources granted (i.e. the set does not change). By default
        * this feature is off, and note that in that case you will not receive the
        * ref\ updateOK() signal. This flag should be set once only before calling anything else
	* (excluding setAutoRelease()), and cannot be unset.
	*/
	bool setAlwaysReply();

	/**
	* Checks whether the always-get-reply flag has been set.
	* \return true if we will always get a reply (even if there is no change).
	*/
	bool alwaysGetReply();

	/**
	* ref\ hasResourcesGranted() returns true if this set has granted resources.
	*/
        bool hasResourcesGranted() { return inAcquireMode; }


signals:
	/**
        * This signal is emitted when the Resource Policy Manager notifies that the given
        * non-granted resources \param availableResources have become available. These are resources
        * are such that the user has shown interest in before. A list of available resources.
        * The list of available resources contains only available resource which we have in the set.
	*/
	void resourcesBecameAvailable(const QList<ResourcePolicy::ResourceType> &availableResources);

	/**
        * This signal is emitted as a response to the acquire() request (also for the update() request
        * when resources has already been granted). Thus, this signal informs  of currently
        * granted resources. \param grantedOptionalResources The list of granted optional
        * resources. All the mandatory resources have also been acquired. Note that this signal
        * is also received after an application with higher priority stopped using the resources
        * that were preempted from you with the lostResources() signal (for this autoRelease must be off).
	*/
	void resourcesGranted(const QList<ResourcePolicy::ResourceType> &grantedOptionalResources);

	/**
        * This signal is emitted as a response to the update() request if the application did not have
        * resources and also in the case when the application did have resources aquired, but the call
        * to update() did not change the set of granted resources.
	* \param grantedOptionalResources The list of granted optional resources.
	* All the mandatory resources have also been updated. Note that a reply to an update()
	* request may also be resourcesLost() if the update request is denied.
	*/
        void updateOK();

	/**
	* This signal is emitted as a response to the acquire() request, in the
	* case where one or more of the mandatory resources were not available.
	*/
	void resourcesDenied();

	/**
	* This signal is emitted as a response to the release() request.
	*/
	void resourcesReleased();

	/**
	* This signal is emitted when the manager releases our acquired resources,
	* so that we have to acquire them again when the user wishes to interact
	* with us.
	*/
	void resourcesReleasedByManager();

	/**
	* This signal is emitted when some other program with a higher priority
	* supersedes us, and as a result we loose all our resources.
	* It is very important to connect to this signal as it is signaling when
        * the acquired resources shouldn't be used anymore. When resourcesGranted() is
        * is emitted againg then the resources can be used again.
	*/
	void lostResources();

	/**
	* Subscribe to this signal to receive error notifications,
	* particularly security errors.
	*/
	void errorCallback(quint32, const char*);

	/**
	* This signals that the manager has started and is available.
	*/
	void managerIsUp();


private:

	bool initialize();

	quint32 identifier;
	const QString resourceClass;
	Resource* resourceSet[NumberOfTypes];
	ResourceEngine*resourceEngine;
	AudioResource*audioResource;
	VideoResource*videoResource;
	bool autoRelease;
	bool alwaysReply;
	bool initialized;
	bool pendingAcquire;
	bool pendingUpdate;
	bool pendingAudioProperties;
	bool pendingVideoProperties;
	bool haveAudioProperties;
	void registerAudioProperties();
	void registerVideoProperties();
	bool inAcquireMode;
	bool proceedIfImFirst( requestType theRequest );
	void executeNextRequest();

	QList<requestType> requestQ;
	QMutex reqMutex;
	bool ignoreQ;

private slots:
	void connectedHandler();
	void handleGranted(quint32);
	void handleDeny();
	void handleReleased();
	void handleReleasedByManager();
	void handleResourcesLost(quint32);
	void handleResourcesBecameAvailable(quint32);
        void handleUpdateOK();
	void handleAudioPropertiesChanged(const QString &group, quint32 pid, const QString &name, const QString &value);
	void handleVideoPropertiesChanged(quint32 pid);

};
}

#endif

