(function() {
	"use strict";

	let modernBridge = null;
	let bridgeLoadPromise = null;
	let lastRenderedMessageCount = 0;
	let lastRenderedTailKey = "";
	let lastScopeToken = "";
	let openMenuId = null;
	let openMenuPinned = false;
	let railCollapsed = false;
	let compactViewport = false;
	let contextMenuState = null;
	let unreadDetachedMessages = 0;
	let selfMenuOpen = false;
	let openReactionPickerMessageId = null;
	let reactionPickerScrollClosePausedUntil = 0;
	let keepMessageListPinnedToBottom = true;
	let pendingBottomPinFrames = 0;
	let pendingBottomPinHandle = 0;
	let footerAlignmentFrame = 0;
	let railLayoutFrame = 0;
	let railSelectionRevealFrame = 0;
	let pendingActiveRailReveal = false;
	let snapshotRenderFrame = 0;
	let menuDismissTimer = 0;
	let appMenuOpen = false;
	let noteExpanded = false;
	let noteUserOverride = false;
	let railNoteAutoCollapsed = false;
	let lastActiveRailToken = "";
	let lastMotdSignature = "";
	let messageListMutationObserver = null;
	let dragState = null;
	let imageViewerDragState = null;
	let imageViewerDragFrame = 0;
	let imageViewerResizeObserver = null;
	let footerAlignmentResizeObserver = null;

	const imageViewerStorageKey = "mumble-modern-image-viewer";
	const imageViewerMinWidth = 280;
	const imageViewerMinHeight = 220;
	const imageViewerViewportMargin = 12;
	const compactRailBreakpointPx = 940;
	const reactionPickerScrollCloseGraceMs = 220;

	const refs = {
		appShell: document.querySelector(".app-shell"),
		modernHeader: document.querySelector(".modern-header"),
		conversationHeader: document.querySelector(".conversation-header"),
		brandTitle: document.getElementById("brand-title"),
		brandSubtitle: document.getElementById("brand-subtitle"),
		menuBar: document.getElementById("menu-bar"),
		railToggleButton: document.getElementById("rail-toggle-button"),
		utilityRail: document.querySelector(".utility-rail"),
		utilityScroll: document.querySelector(".utility-scroll"),
		utilityRailBackdrop: document.getElementById("utility-rail-backdrop"),
		railCloseButton: document.getElementById("rail-close-button"),
		noteCard: document.querySelector(".note-card"),
		noteToggleButton: document.getElementById("note-toggle-button"),
		serverEyebrow: document.getElementById("server-eyebrow"),
		serverTitle: document.getElementById("server-title"),
		serverSubtitle: document.getElementById("server-subtitle"),
		layoutPill: document.getElementById("layout-pill"),
		connectionPill: document.getElementById("connection-pill"),
		compatPill: document.getElementById("compat-pill"),
		connectButton: document.getElementById("connect-button"),
		disconnectButton: document.getElementById("disconnect-button"),
		layoutSwitchButton: document.getElementById("layout-switch-button"),
		settingsButton: document.getElementById("settings-button"),
		muteButton: document.getElementById("mute-button"),
		deafButton: document.getElementById("deaf-button"),
		textRoomCount: document.getElementById("text-room-count"),
		voiceRoomCount: document.getElementById("voice-room-count"),
		textRoomList: document.getElementById("text-room-list"),
		voiceRoomList: document.getElementById("voice-room-list"),
		scopeTitle: document.getElementById("scope-title"),
		scopeDescription: document.getElementById("scope-description"),
		scopeBanner: document.getElementById("scope-banner"),
		conversationMeta: document.getElementById("conversation-meta"),
		voicePresenceStack: document.getElementById("voice-presence-stack"),
		screenShareButton: document.getElementById("screen-share-button"),
		screenShareCard: document.getElementById("screen-share-card"),
		screenShareCardStatus: document.getElementById("screen-share-card-status"),
		screenShareCardTitle: document.getElementById("screen-share-card-title"),
		screenShareCardSummary: document.getElementById("screen-share-card-summary"),
		screenShareCardNote: document.getElementById("screen-share-card-note"),
		screenShareCardActions: document.getElementById("screen-share-card-actions"),
		messageList: document.getElementById("message-list"),
		jumpLatestButton: document.getElementById("jump-latest-button"),
		composerForm: document.getElementById("composer-form"),
		composerShell: document.querySelector(".composer-shell"),
		composerReplyBar: document.getElementById("composer-reply-bar"),
		composerReplyLabel: document.getElementById("composer-reply-label"),
		composerReplySnippet: document.getElementById("composer-reply-snippet"),
		composerReplyCancelButton: document.getElementById("composer-reply-cancel-button"),
		composerInput: document.getElementById("composer-input"),
		composerHint: document.getElementById("composer-hint"),
		attachButton: document.getElementById("attach-button"),
		sendButton: document.getElementById("send-button"),
		loadOlderButton: document.getElementById("load-older-button"),
		markReadButton: document.getElementById("mark-read-button"),
		selfCard: document.getElementById("self-card"),
		selfCardSettingsButton: document.getElementById("self-card-settings-button"),
		selfAvatar: document.getElementById("self-avatar"),
		selfCopy: document.querySelector(".self-copy"),
		selfName: document.getElementById("self-name"),
		selfStatus: document.getElementById("self-status"),
		appMenu: document.getElementById("app-menu-popover"),
		selfMenu: document.getElementById("self-menu-popover"),
		contextMenu: document.getElementById("context-menu"),
		imageViewerLayer: document.getElementById("image-viewer-layer"),
		imageViewerBackdrop: document.getElementById("image-viewer-backdrop"),
		imageViewerWindow: document.getElementById("image-viewer-window"),
		imageViewerHeader: document.getElementById("image-viewer-header"),
		imageViewerTitle: document.getElementById("image-viewer-title"),
		imageViewerCloseButton: document.getElementById("image-viewer-close-button"),
		imageViewerImage: document.getElementById("image-viewer-image")
	};
	const starterReactionEmoji = ["👍", "❤️", "😂", "😮", "😢", "🎉", "👀", "🔥"];
	let imageViewerState = loadImageViewerState();

	function viewportMatchesCompactRail() {
		return window.innerWidth <= compactRailBreakpointPx;
	}

	function applyRailPresentation() {
		const railOpen = !railCollapsed;
		refs.appShell.classList.toggle("is-compact-layout", compactViewport);
		refs.appShell.classList.toggle("rail-is-collapsed", railCollapsed);
		refs.railToggleButton.classList.toggle("is-active", railOpen);
		refs.railToggleButton.textContent = compactViewport && railOpen ? "Close" : "Rooms";
		refs.railToggleButton.title = compactViewport && railOpen ? "Close room browser" : "Open room browser";
		refs.railToggleButton.setAttribute("aria-label", compactViewport && railOpen ? "Close room browser" : "Open room browser");
		refs.railToggleButton.setAttribute("aria-expanded", railOpen ? "true" : "false");
		refs.utilityRail.setAttribute("aria-hidden", compactViewport && !railOpen ? "true" : "false");
		refs.utilityRailBackdrop.classList.toggle("hidden", !(compactViewport && railOpen));
		refs.utilityRailBackdrop.setAttribute("aria-hidden", compactViewport && railOpen ? "false" : "true");
	}

	function setRailCollapsed(nextCollapsed) {
		const normalized = !!nextCollapsed;
		if (railCollapsed === normalized) {
			applyRailPresentation();
			return;
		}

		railCollapsed = normalized;
		if (!railCollapsed) {
			hideContextMenu();
			hideSelfMenu();
			pendingActiveRailReveal = true;
			scheduleRailLayoutSync();
		}
		applyRailPresentation();
	}

	function syncCompactRailState(force) {
		const nextCompactViewport = viewportMatchesCompactRail();
		if (force || nextCompactViewport !== compactViewport) {
			compactViewport = nextCompactViewport;
			railCollapsed = compactViewport ? true : false;
		}
		applyRailPresentation();
	}

	function measureRailSection(section, listElement, itemCount) {
		if (!section || !listElement || section.classList.contains("hidden")) {
			return null;
		}

		const listRect = listElement.getBoundingClientRect();
		const sectionRect = section.getBoundingClientRect();
		const rootLabel = listElement.querySelector(".rail-root-label");
		const row = listElement.querySelector(".rail-row, .rail-empty");
		const rootHeight = rootLabel ? Math.ceil(rootLabel.getBoundingClientRect().height) : 0;
		const fallbackRowHeight = compactViewport ? 36 : 40;
		const rowHeight = row ? Math.max(Math.ceil(row.getBoundingClientRect().height), fallbackRowHeight) : fallbackRowHeight;
		const targetRows = itemCount > 0 ? Math.min(itemCount, (compactViewport || noteExpanded) ? 2 : 3) : 1;
		const listMinHeight = rootHeight + (rowHeight * Math.max(targetRows, 1)) + 8;
		const chromeHeight = Math.max(26, Math.ceil(sectionRect.height - listRect.height));
		return {
			section: section,
			listElement: listElement,
			listMinHeight: listMinHeight,
			sectionMinHeight: chromeHeight + listMinHeight
		};
	}

	function scheduleRailLayoutSync() {
		if (railLayoutFrame) {
			return;
		}

		railLayoutFrame = requestAnimationFrame(function() {
			railLayoutFrame = 0;
			syncRailLayout();
		});
	}

	function railOverflowAmount() {
		if (!refs.utilityScroll) {
			return 0;
		}

		return Math.max(0, refs.utilityScroll.scrollHeight - refs.utilityScroll.clientHeight);
	}

	function syncRailOverflowState() {
		if (!refs.appShell || !refs.utilityScroll) {
			return 0;
		}

		const overflow = railOverflowAmount();
		refs.appShell.classList.toggle("rail-has-overflow", overflow > 1);
		refs.appShell.classList.toggle("rail-note-auto-compacted", railNoteAutoCollapsed);
		return overflow;
	}

	function scheduleActiveRailRowReveal() {
		if (railSelectionRevealFrame) {
			return;
		}

		railSelectionRevealFrame = requestAnimationFrame(function() {
			railSelectionRevealFrame = 0;
			revealActiveRailRow();
		});
	}

	function revealActiveRailRow() {
		if (!refs.utilityScroll) {
			return;
		}

		const activeRow = activeRailRow();
		if (!activeRow) {
			return;
		}

		const railRect = refs.utilityScroll.getBoundingClientRect();
		const rowRect = activeRow.getBoundingClientRect();
		const topGuard = 12;
		const bottomGuard = 16;
		if (rowRect.top < railRect.top + topGuard || rowRect.bottom > railRect.bottom - bottomGuard) {
			activeRow.scrollIntoView({ block: "nearest", inline: "nearest" });
		}
	}

	function activeRailRow() {
		if (!refs.utilityScroll) {
			return null;
		}

		return refs.utilityScroll.querySelector(".rail-row.is-selected")
			|| refs.utilityScroll.querySelector(".rail-row.is-joined");
	}

	function activeRailToken() {
		const row = activeRailRow();
		return row ? String(row.dataset.scopeToken || row.dataset.roomLabel || "") : "";
	}

	function syncRailLayout() {
		if (!refs.appShell || !refs.utilityScroll || !refs.noteCard || !refs.serverSubtitle
			|| !refs.voiceRoomList || !refs.textRoomList) {
			return;
		}

		const utilityHeight = Math.ceil(refs.utilityScroll.clientHeight || 0);
		if (utilityHeight <= 0) {
			return;
		}

		const voiceSection = refs.voiceRoomList.closest(".room-card-block");
		const textSection = refs.textRoomList.closest(".room-card-block");
		const sections = [
			measureRailSection(voiceSection, refs.voiceRoomList, Number(refs.voiceRoomCount.textContent || 0)),
			measureRailSection(textSection, refs.textRoomList, Number(refs.textRoomCount.textContent || 0))
		].filter(function(entry) {
			return !!entry;
		});

		sections.forEach(function(entry) {
			entry.listElement.style.minHeight = "";
			entry.section.style.minHeight = "";
			entry.section.style.flexBasis = "";
		});

		if (refs.noteCard.classList.contains("hidden")) {
			refs.noteCard.style.maxHeight = "";
			refs.serverSubtitle.style.maxHeight = "";
			railNoteAutoCollapsed = false;
			syncRailOverflowState();
			if (pendingActiveRailReveal) {
				pendingActiveRailReveal = false;
				scheduleActiveRailRowReveal();
			}
			return;
		}

		const noteCardRect = refs.noteCard.getBoundingClientRect();
		const noteBodyRect = refs.serverSubtitle.getBoundingClientRect();
		const noteChromeHeight = Math.max(52, Math.ceil(noteCardRect.height - noteBodyRect.height));
		const minimumNoteBodyHeight = noteExpanded ? (compactViewport ? 92 : 124) : (compactViewport ? 52 : 72);
		const sectionGapBudget = sections.length > 0 ? 10 : 0;
		const minimumSectionBudget = sections.reduce(function(total, entry) {
			return total + entry.sectionMinHeight;
		}, 0);
		const noteBudget = utilityHeight - minimumSectionBudget - sectionGapBudget;
		const desiredNoteMaxHeight = noteExpanded
			? Math.round(Math.max(compactViewport ? 208 : 260,
				Math.min(compactViewport ? 272 : 360, utilityHeight * (compactViewport ? 0.44 : 0.5))))
			: (compactViewport ? 160 : 220);
		const minimumNoteMaxHeight = noteChromeHeight + minimumNoteBodyHeight;
		const availableNoteBudget = Math.max(minimumNoteMaxHeight, noteBudget);
		const clampedNoteMaxHeight = Math.max(minimumNoteMaxHeight, Math.min(desiredNoteMaxHeight, availableNoteBudget));
		const noteBodyMaxHeight = Math.max(minimumNoteBodyHeight, clampedNoteMaxHeight - noteChromeHeight);

		refs.noteCard.style.maxHeight = Math.round(clampedNoteMaxHeight) + "px";
		refs.serverSubtitle.style.maxHeight = Math.round(noteBodyMaxHeight) + "px";

		const roomCount = Number(refs.voiceRoomCount.textContent || 0) + Number(refs.textRoomCount.textContent || 0);
		const overflow = syncRailOverflowState();
		if (overflow > 12 && roomCount > 0 && noteExpanded && !noteUserOverride) {
			railNoteAutoCollapsed = true;
			noteExpanded = false;
			renderNote(getSnapshot().app || {}, getSnapshot().activeScope || {});
			return;
		}

		if (pendingActiveRailReveal) {
			pendingActiveRailReveal = false;
			scheduleActiveRailRowReveal();
		}
	}

	function normalizeWheelDelta(event) {
		let deltaY = Number(event.deltaY || 0);
		if (!deltaY) {
			return 0;
		}

		if (event.deltaMode === 1) {
			deltaY *= 16;
		} else if (event.deltaMode === 2) {
			deltaY *= Math.max(1, refs.utilityScroll ? refs.utilityScroll.clientHeight : window.innerHeight);
		}
		return deltaY;
	}

	function canScrollElement(element, deltaY) {
		if (!element || Math.abs(deltaY) < 0.01) {
			return false;
		}

		const maxScrollTop = Math.max(0, element.scrollHeight - element.clientHeight);
		if (maxScrollTop <= 1) {
			return false;
		}

		return deltaY < 0 ? element.scrollTop > 0 : element.scrollTop < maxScrollTop - 1;
	}

	function findNestedRailScroller(target, deltaY) {
		let element = target && target.nodeType === Node.ELEMENT_NODE ? target : target && target.parentElement;
		while (element && element !== refs.utilityScroll && element !== document.body) {
			const style = window.getComputedStyle(element);
			if ((style.overflowY === "auto" || style.overflowY === "scroll") && canScrollElement(element, deltaY)) {
				return element;
			}
			element = element.parentElement;
		}
		return null;
	}

	function handleUtilityWheel(event) {
		if (!refs.utilityScroll || event.defaultPrevented || event.ctrlKey || event.metaKey || event.altKey) {
			return;
		}

		const deltaY = normalizeWheelDelta(event);
		if (!deltaY || Math.abs(Number(event.deltaX || 0)) > Math.abs(deltaY)) {
			return;
		}

		if (findNestedRailScroller(event.target, deltaY)) {
			return;
		}

		if (!canScrollElement(refs.utilityScroll, deltaY)) {
			return;
		}

		event.preventDefault();
		refs.utilityScroll.scrollTop = Math.max(0, Math.min(
			refs.utilityScroll.scrollTop + deltaY,
			refs.utilityScroll.scrollHeight - refs.utilityScroll.clientHeight
		));
		hideContextMenu();
		hideSelfMenu();
		syncRailOverflowState();
	}

	function dismissCompactRailAfterAction() {
		if (!compactViewport || railCollapsed) {
			return;
		}
		setRailCollapsed(true);
	}

	function notifyBridge(method) {
		if (!modernBridge || typeof modernBridge[method] !== "function") {
			return;
		}

		const args = Array.prototype.slice.call(arguments, 1);
		try {
			modernBridge[method].apply(modernBridge, args);
		} catch (error) {
			console.warn("Modern bridge call failed:", method, error);
		}
	}

	async function ensureBridge() {
		if (!window.qt || !window.qt.webChannelTransport) {
			return;
		}

		if (modernBridge) {
			return;
		}

		async function bindBridge() {
			return new Promise(function(resolve) {
				try {
					new QWebChannel(qt.webChannelTransport, function(channel) {
						modernBridge = channel.objects.modernBridge || null;
						if (modernBridge) {
							if (modernBridge.snapshotChanged && typeof modernBridge.snapshotChanged.connect === "function") {
								modernBridge.snapshotChanged.connect(syncSnapshot);
							}
							notifyBridge("ready");
							syncSnapshot();
						}
						resolve();
					});
				} catch (error) {
					console.warn("Modern bridge initialization failed:", error);
					resolve();
				}
			});
		}

		if (window.QWebChannel) {
			await bindBridge();
			return;
		}

		if (!bridgeLoadPromise) {
			bridgeLoadPromise = new Promise(function(resolve) {
				const script = document.createElement("script");
				script.src = "qrc:///qtwebchannel/qwebchannel.js";
				script.async = true;
				script.onload = function() {
					bindBridge().then(resolve);
				};
				script.onerror = function() {
					console.warn("Unable to load qwebchannel.js for the modern layout.");
					resolve();
				};
				document.head.appendChild(script);
			});
		}

		await bridgeLoadPromise;
	}

	function getSnapshot() {
		return modernBridge ? (modernBridge.snapshot || {}) : {};
	}

	function scheduleSnapshotRender() {
		if (snapshotRenderFrame) {
			return;
		}

		snapshotRenderFrame = requestAnimationFrame(function() {
			snapshotRenderFrame = 0;
			syncCompactRailState(false);
			render(getSnapshot());
		});
	}

	function plainTextFromHtml(html) {
		const template = document.createElement("template");
		template.innerHTML = String(html || "");
		return String(template.content.textContent || "").replace(/\s+/g, " ").trim();
	}

	function escapedMultilineText(value) {
		return escapeHtml(value).replace(/\n/g, "<br>");
	}

	function escapeHtml(value) {
		return String(value || "")
			.replace(/&/g, "&amp;")
			.replace(/</g, "&lt;")
			.replace(/>/g, "&gt;")
			.replace(/\"/g, "&quot;");
	}

	function initialsFor(label) {
		const parts = String(label || "").trim().split(/\s+/).filter(Boolean);
		if (!parts.length) {
			return "?";
		}
		if (parts.length === 1) {
			return parts[0].slice(0, 1).toUpperCase();
		}
		return (parts[0].slice(0, 1) + parts[1].slice(0, 1)).toUpperCase();
	}

	function hueForLabel(label, own) {
		if (own) {
			return 173;
		}

		let hash = 0;
		const source = String(label || "");
		for (let index = 0; index < source.length; index += 1) {
			hash = ((hash << 5) - hash) + source.charCodeAt(index);
			hash |= 0;
		}

		return Math.abs(hash) % 360;
	}

	function styleAvatar(element, label, own, avatarUrl) {
		const hue = hueForLabel(label, own);
		element.style.setProperty("--avatar-hue", String(hue));
		if (avatarUrl) {
			element.classList.add("has-image");
			element.style.backgroundImage = "url(\"" + String(avatarUrl).replace(/"/g, "%22") + "\")";
			element.textContent = "";
			return;
		}

		element.classList.remove("has-image");
		element.style.backgroundImage = "";
		element.textContent = initialsFor(label);
	}

	function applyStatePill(element, label, tone) {
		element.textContent = label;
		element.className = "state-pill";
		if (tone) {
			element.classList.add("is-" + tone);
		}
	}

	function kindChipText(kindLabel) {
		switch (String(kindLabel || "").toLowerCase()) {
			case "activity":
				return "LOG";
			case "voice room":
				return "VC";
			case "text room":
				return "TXT";
			case "direct message":
				return "DM";
			default:
				return "TXT";
		}
	}

	function eventElementTarget(event) {
		if (!event || !event.target) {
			return null;
		}

		if (event.target.nodeType === Node.ELEMENT_NODE) {
			return event.target;
		}

		return event.target.parentElement || null;
	}

	function anchorFromEvent(event) {
		const target = eventElementTarget(event);
		if (!target || typeof target.closest !== "function") {
			return null;
		}

		return target.closest("a[href]");
	}

	function imageFromEvent(event) {
		const target = eventElementTarget(event);
		if (!target || typeof target.closest !== "function") {
			return null;
		}

		const image = target.closest("img");
		if (!image || !refs.messageList || !refs.messageList.contains(image)) {
			return null;
		}

		return image;
	}

	function isInlineDataImageOpenHref(href) {
		return /^mumble-chat:\/\/inline-data-image\//i.test(String(href || "").trim());
	}

	function handleAnchorActivation(event) {
		const image = imageFromEvent(event);
		const imageAnchor = image ? anchorFromEvent(event) : null;
		if (image && imageAnchor) {
			const imageHref = String(imageAnchor.getAttribute("href") || imageAnchor.href || "").trim();
			event.preventDefault();
			event.stopPropagation();
			hideContextMenu();
			hideSelfMenu();
			if (isInlineDataImageOpenHref(imageHref) && modernBridge && typeof modernBridge.activateLink === "function") {
				notifyBridge("activateLink", imageHref);
				return;
			}
			openImageViewerFromElement(image);
			return;
		}

		if (!modernBridge || typeof modernBridge.activateLink !== "function") {
			return;
		}

		const anchor = anchorFromEvent(event);
		if (!anchor) {
			return;
		}

		const href = String(anchor.href || anchor.getAttribute("href") || "").trim();
		if (!href) {
			return;
		}

		event.preventDefault();
		event.stopPropagation();
		hideContextMenu();
		hideSelfMenu();
		notifyBridge("activateLink", href);
	}

	function defaultImageViewerState() {
		const width = Math.min(560, Math.max(imageViewerMinWidth, window.innerWidth - 64));
		const height = Math.min(400, Math.max(imageViewerMinHeight, window.innerHeight - 88));
		return {
			width: Math.round(width),
			height: Math.round(height),
			left: Math.round((window.innerWidth - width) / 2),
			top: Math.max(20, Math.round((window.innerHeight - height) / 2) - 24)
		};
	}

	function clampImageViewerState(state) {
		const fallback = defaultImageViewerState();
		const requestedWidth = Number(state && state.width);
		const requestedHeight = Number(state && state.height);
		const requestedLeft = Number(state && state.left);
		const requestedTop = Number(state && state.top);
		const maxWidth = Math.max(220, window.innerWidth - (imageViewerViewportMargin * 2));
		const maxHeight = Math.max(180, window.innerHeight - (imageViewerViewportMargin * 2));
		const minWidth = Math.min(imageViewerMinWidth, maxWidth);
		const minHeight = Math.min(imageViewerMinHeight, maxHeight);
		const width = Math.round(Math.min(maxWidth, Math.max(minWidth, isFinite(requestedWidth) ? requestedWidth : fallback.width)));
		const height = Math.round(Math.min(maxHeight, Math.max(minHeight, isFinite(requestedHeight) ? requestedHeight : fallback.height)));
		const maxLeft = Math.max(imageViewerViewportMargin, window.innerWidth - width - imageViewerViewportMargin);
		const maxTop = Math.max(imageViewerViewportMargin, window.innerHeight - height - imageViewerViewportMargin);
		return {
			width: width,
			height: height,
			left: Math.round(Math.min(maxLeft, Math.max(imageViewerViewportMargin, isFinite(requestedLeft) ? requestedLeft : fallback.left))),
			top: Math.round(Math.min(maxTop, Math.max(imageViewerViewportMargin, isFinite(requestedTop) ? requestedTop : fallback.top)))
		};
	}

	function loadImageViewerState() {
		try {
			const raw = window.localStorage ? window.localStorage.getItem(imageViewerStorageKey) : "";
			if (!raw) {
				return clampImageViewerState(defaultImageViewerState());
			}
			return clampImageViewerState(JSON.parse(raw));
		} catch (error) {
			return clampImageViewerState(defaultImageViewerState());
		}
	}

	function persistImageViewerState() {
		if (!imageViewerState) {
			return;
		}

		try {
			if (window.localStorage) {
				window.localStorage.setItem(imageViewerStorageKey, JSON.stringify(imageViewerState));
			}
		} catch (error) {}
	}

	function applyImageViewerGeometry() {
		if (!refs.imageViewerWindow || !imageViewerState) {
			return;
		}

		imageViewerState = clampImageViewerState(imageViewerState);
		refs.imageViewerWindow.style.width = imageViewerState.width + "px";
		refs.imageViewerWindow.style.height = imageViewerState.height + "px";
		refs.imageViewerWindow.style.left = imageViewerState.left + "px";
		refs.imageViewerWindow.style.top = imageViewerState.top + "px";
		if (!imageViewerDragState) {
			refs.imageViewerWindow.style.transform = "";
		}
	}

	function clearImageViewerDragFrame() {
		if (!imageViewerDragFrame) {
			return;
		}

		cancelAnimationFrame(imageViewerDragFrame);
		imageViewerDragFrame = 0;
	}

	function applyImageViewerDragPreview() {
		imageViewerDragFrame = 0;
		if (!refs.imageViewerWindow || !imageViewerDragState) {
			return;
		}

		const offsetLeft = Math.round(imageViewerDragState.currentLeft - imageViewerDragState.startLeft);
		const offsetTop = Math.round(imageViewerDragState.currentTop - imageViewerDragState.startTop);
		refs.imageViewerWindow.style.transform = "translate3d(" + offsetLeft + "px, " + offsetTop + "px, 0)";
	}

	function scheduleImageViewerDragPreview() {
		if (imageViewerDragFrame) {
			return;
		}

		imageViewerDragFrame = requestAnimationFrame(applyImageViewerDragPreview);
	}

	function stopObservingImageViewerSize() {
		if (!imageViewerResizeObserver) {
			return;
		}

		imageViewerResizeObserver.disconnect();
		imageViewerResizeObserver = null;
	}

	function observeImageViewerSize() {
		if (!refs.imageViewerWindow || typeof ResizeObserver !== "function") {
			return;
		}

		stopObservingImageViewerSize();
		imageViewerResizeObserver = new ResizeObserver(function() {
			const rect = refs.imageViewerWindow.getBoundingClientRect();
			const nextState = clampImageViewerState({
				width: rect.width,
				height: rect.height,
				left: imageViewerState ? imageViewerState.left : rect.left,
				top: imageViewerState ? imageViewerState.top : rect.top
			});
			const moved = !imageViewerState || nextState.left !== imageViewerState.left || nextState.top !== imageViewerState.top;
			const resized = !imageViewerState || nextState.width !== imageViewerState.width || nextState.height !== imageViewerState.height;
			imageViewerState = nextState;
			if (moved) {
				applyImageViewerGeometry();
			}
			if (moved || resized) {
				persistImageViewerState();
			}
		});
		imageViewerResizeObserver.observe(refs.imageViewerWindow);
	}

	function endImageViewerDrag(commitGeometry) {
		if (!imageViewerDragState) {
			return false;
		}

		imageViewerDragState = null;
		clearImageViewerDragFrame();
		document.body.classList.remove("image-viewer-dragging");
		window.removeEventListener("mousemove", trackImageViewerDrag, true);
		window.removeEventListener("mouseup", finishImageViewerDrag, true);
		if (commitGeometry) {
			applyImageViewerGeometry();
		} else if (refs.imageViewerWindow) {
			refs.imageViewerWindow.style.transform = "";
		}
		return true;
	}

	function cancelImageViewerDrag() {
		endImageViewerDrag(false);
	}

	function finishImageViewerDrag() {
		if (endImageViewerDrag(true)) {
			persistImageViewerState();
		}
	}

	function trackImageViewerDrag(event) {
		if (!imageViewerDragState) {
			return;
		}

		imageViewerState = clampImageViewerState({
			width: imageViewerState.width,
			height: imageViewerState.height,
			left: imageViewerDragState.startLeft + (event.clientX - imageViewerDragState.startX),
			top: imageViewerDragState.startTop + (event.clientY - imageViewerDragState.startY)
		});
		imageViewerDragState.currentLeft = imageViewerState.left;
		imageViewerDragState.currentTop = imageViewerState.top;
		scheduleImageViewerDragPreview();
		event.preventDefault();
	}

	function beginImageViewerDrag(event) {
		if (event.button !== 0 || !refs.imageViewerWindow || !refs.imageViewerLayer || refs.imageViewerLayer.classList.contains("hidden")) {
			return;
		}
		const dragTarget = eventElementTarget(event);
		if (dragTarget && typeof dragTarget.closest === "function" && dragTarget.closest("button")) {
			return;
		}

		const rect = refs.imageViewerWindow.getBoundingClientRect();
		imageViewerDragState = {
			startX: event.clientX,
			startY: event.clientY,
			startLeft: rect.left,
			startTop: rect.top,
			currentLeft: rect.left,
			currentTop: rect.top
		};
		document.body.classList.add("image-viewer-dragging");
		window.addEventListener("mousemove", trackImageViewerDrag, true);
		window.addEventListener("mouseup", finishImageViewerDrag, true);
		event.preventDefault();
	}

	function openImageViewer(source, title) {
		const normalizedSource = String(source || "").trim();
		if (!normalizedSource || !refs.imageViewerLayer || !refs.imageViewerImage) {
			return false;
		}

		imageViewerState = clampImageViewerState(imageViewerState || defaultImageViewerState());
		refs.imageViewerTitle.textContent = String(title || "").trim() || "Image";
		refs.imageViewerImage.src = normalizedSource;
		refs.imageViewerImage.alt = String(title || "").trim() || "Image";
		refs.imageViewerLayer.classList.remove("hidden");
		refs.imageViewerLayer.setAttribute("aria-hidden", "false");
		applyImageViewerGeometry();
		observeImageViewerSize();
		return true;
	}

	function closeImageViewer() {
		if (!refs.imageViewerLayer || refs.imageViewerLayer.classList.contains("hidden")) {
			return;
		}

		cancelImageViewerDrag();
		stopObservingImageViewerSize();
		refs.imageViewerLayer.classList.add("hidden");
		refs.imageViewerLayer.setAttribute("aria-hidden", "true");
		refs.imageViewerImage.removeAttribute("src");
		persistImageViewerState();
	}

	function openImageViewerFromElement(image) {
		if (!image) {
			return false;
		}

		const source = String(image.currentSrc || image.src || image.getAttribute("src") || "").trim();
		if (!source) {
			return false;
		}

		return openImageViewer(source, image.getAttribute("alt") || "");
	}

	function handleMessageImageActivation(event) {
		const image = imageFromEvent(event);
		if (!image) {
			return;
		}

		event.preventDefault();
		event.stopPropagation();
		hideContextMenu();
		hideSelfMenu();
		openImageViewerFromElement(image);
	}

	function dayLabelFromMs(createdAtMs) {
		if (!createdAtMs) {
			return "";
		}

		const target = new Date(Number(createdAtMs));
		const now = new Date();
		const today = new Date(now.getFullYear(), now.getMonth(), now.getDate());
		const yesterday = new Date(today);
		yesterday.setDate(today.getDate() - 1);
		const targetDay = new Date(target.getFullYear(), target.getMonth(), target.getDate());

		if (targetDay.getTime() === today.getTime()) {
			return "TODAY";
		}
		if (targetDay.getTime() === yesterday.getTime()) {
			return "YESTERDAY";
		}

		return target.toLocaleDateString(undefined, { month: "short", day: "numeric" }).toUpperCase();
	}

	function isSameDay(leftMs, rightMs) {
		if (!leftMs || !rightMs) {
			return false;
		}

		const left = new Date(Number(leftMs));
		const right = new Date(Number(rightMs));
		return left.getFullYear() === right.getFullYear()
			&& left.getMonth() === right.getMonth()
			&& left.getDate() === right.getDate();
	}

	function shouldGroupWith(previous, current) {
		if (!previous || !current || previous.system || current.system) {
			return false;
		}

		if (!!previous.own !== !!current.own) {
			return false;
		}

		if (String(previous.actor || "") !== String(current.actor || "")) {
			return false;
		}

		if (!isSameDay(previous.createdAtMs, current.createdAtMs)) {
			return false;
		}

		if (!previous.createdAtMs || !current.createdAtMs) {
			return true;
		}

		const gapMs = Math.abs(Number(current.createdAtMs) - Number(previous.createdAtMs));
		return gapMs <= (8 * 60 * 1000);
	}

	function renderMeta(meta) {
		refs.conversationMeta.innerHTML = "";
		(meta || []).forEach(function(entry) {
			const pill = document.createElement("span");
			pill.className = "meta-pill";
			pill.textContent = entry;
			refs.conversationMeta.appendChild(pill);
		});
	}

	function screenShareVisible(share) {
		return !!(share && share.visible);
	}

	function screenShareStatusText(share) {
		switch (String(share && share.mode || "")) {
			case "publishing":
				return "Publishing";
			case "available":
				return "Available";
			case "viewing":
				return "Viewing";
			case "fallback":
				return "Fallback";
			case "error":
				return "Unavailable";
			default:
				return "Idle";
		}
	}

	function compactScreenShareActionLabel(label) {
		switch (String(label || "").toLowerCase()) {
			case "share screen":
				return "Share";
			case "watch share":
				return "Watch";
			case "open share window":
				return "Open";
			default:
				return label || "Share";
		}
	}

	function renderScreenShareHeader(scope, share) {
		const visible = screenShareVisible(share);
		const active = String(share && share.mode || "") === "publishing"
			|| String(share && share.mode || "") === "viewing"
			|| String(share && share.mode || "") === "fallback";
		const buttonClasses = ["chip-button", "screen-share-button"];
		if (!visible) {
			buttonClasses.push("hidden");
		} else if (share.primaryTone) {
			buttonClasses.push("is-" + share.primaryTone);
		}
		if (active) {
			buttonClasses.push("is-active");
		}
		refs.screenShareButton.className = buttonClasses.join(" ");
		if (!visible) {
			refs.screenShareButton.disabled = true;
			refs.screenShareButton.textContent = "Share screen";
			refs.screenShareButton.title = "Share screen";
			refs.screenShareButton.dataset.scopeToken = "";
			refs.screenShareButton.dataset.actionId = "";
			return;
		}

		const label = share.primaryLabel || "Share screen";
		const hint = share.primaryHint || label;
		refs.screenShareButton.textContent = label;
		refs.screenShareButton.disabled = share.primaryEnabled === false;
		refs.screenShareButton.title = hint;
		refs.screenShareButton.dataset.scopeToken = scope.scopeToken || share.scopeToken || "";
		refs.screenShareButton.dataset.actionId = share.primaryActionId || "";
	}

	function renderScreenShareCard(scope, share) {
		const visible = screenShareVisible(share);
		const mode = String(share && share.mode || "idle");
		const statusTone = share && share.statusTone ? " is-" + share.statusTone : "";
		refs.screenShareCard.className = "screen-share-card" + statusTone + (mode ? " is-mode-" + mode : "")
			+ (visible ? "" : " hidden");
		if (!visible) {
			refs.screenShareCardActions.innerHTML = "";
			return;
		}

		let title = "Screen sharing";
		if (mode === "publishing") {
			title = "Your screen share";
		} else if ((mode === "available" || mode === "viewing" || mode === "fallback") && share.ownerLabel) {
			title = share.ownerLabel;
		} else if (mode === "idle") {
			title = "No active share";
		}

		refs.screenShareCardStatus.className = "meta-pill" + statusTone + (share.statusLabel ? "" : " hidden");
		refs.screenShareCardStatus.textContent = screenShareStatusText(share);
		refs.screenShareCardTitle.textContent = title;
		refs.screenShareCardSummary.textContent = share.statusLabel || "Screen sharing is available in voice rooms.";

		const note = share.fallbackLabel || ((share.primaryEnabled === false && share.primaryHint) ? share.primaryHint : "");
		refs.screenShareCardNote.textContent = note;
		refs.screenShareCardNote.classList.toggle("hidden", !note);

		refs.screenShareCardActions.innerHTML = "";
		actionItemsFromActionStates(share.overflowActions, {
			invokeAction: function(actionId) {
				notifyBridge("invokeScopeAction", scope.scopeToken || share.scopeToken || "", actionId);
			}
		}).forEach(function(item) {
			if (!item || item.kind !== "action") {
				return;
			}

			const button = document.createElement("button");
			button.type = "button";
			button.className = "chip-button screen-share-card-action"
				+ (item.tone ? " is-" + item.tone : "");
			button.textContent = item.label || "Action";
			button.disabled = item.enabled === false;
			if (item.hint) {
				button.title = item.hint;
			}
			button.addEventListener("click", function() {
				if (item.enabled === false || typeof item.action !== "function") {
					return;
				}
				item.action();
			});
			refs.screenShareCardActions.appendChild(button);
		});
	}

	function findRoomState(snapshot, scopeToken) {
		const token = String(scopeToken || "");
		return (snapshot.voiceRooms || []).concat(snapshot.textRooms || []).find(function(room) {
			return String(room.token || "") === token;
		}) || null;
	}

	function participantStateKey(person) {
		if (!person) {
			return "";
		}

		return String(person.participantKey || person.session || "");
	}

	function findParticipantState(snapshot, key) {
		const targetKey = String(key || "");
		const directMatch = (snapshot.voicePresence || []).concat(snapshot.participants || []).find(function(person) {
			return participantStateKey(person) === targetKey;
		});
		if (directMatch) {
			return directMatch;
		}

		for (const room of (snapshot.voiceRooms || [])) {
			const roomMatch = (room.participants || []).find(function(person) {
				return participantStateKey(person) === targetKey;
			});
			if (roomMatch) {
				return roomMatch;
			}
		}

		return null;
	}

	function findMessageState(snapshot, messageId) {
		const targetId = String(messageId || "");
		return (snapshot.messages || []).find(function(message) {
			return String(message.messageId || "") === targetId;
		}) || null;
	}

	function actionItemsFromActionStates(actionStates, handlers) {
		return (actionStates || []).map(function(item) {
			if (!item) {
				return null;
			}

			const kind = String(item.kind || "action");
			if (kind === "separator") {
				return { kind: "separator", separator: true };
			}

			if (kind === "label") {
				return {
					kind: "label",
					label: item.label || "",
					hint: item.hint || ""
				};
			}

			if (kind === "slider") {
				if (!item.id) {
					return null;
				}

				return {
					kind: "slider",
					id: item.id || "",
					label: item.label || "Value",
					enabled: item.enabled !== false,
					tone: item.tone || "",
					hint: item.hint || "",
					value: Number(item.value || 0),
					min: Number(item.min || 0),
					max: Number(item.max || 0),
					step: Number(item.step || 1),
					suffix: item.suffix || "",
					finalOnRelease: item.finalOnRelease !== false,
					onValueChange: function(value, final) {
						if (handlers && typeof handlers.invokeValueChanged === "function") {
							handlers.invokeValueChanged(item.id || "", value, final);
						}
					}
				};
			}

			if (!item.id) {
				return null;
			}

			return {
				kind: "action",
				label: item.label || "Action",
				enabled: item.enabled !== false,
				checked: !!item.checked,
				tone: item.tone || "",
				hint: item.hint || "",
				action: function() {
					if (handlers && typeof handlers.invokeAction === "function") {
						handlers.invokeAction(item.id || "");
					}
				}
			};
		});
	}

	function actionPanelItemKind(item) {
		if (!item) {
			return "";
		}

		if (item.kind) {
			return String(item.kind);
		}
		if (item.separator) {
			return "separator";
		}
		return "action";
	}

	function sliderValueLabel(item, value) {
		const numericValue = Number(value || 0);
		if (!Number.isFinite(numericValue)) {
			return "";
		}

		const prefix = numericValue > 0 ? "+" : "";
		return prefix + String(numericValue) + String(item && item.suffix ? item.suffix : "");
	}

	function appendActionPanelItem(container, item, variant, hideOnAction) {
		const prefix = variant === "menu" ? "menu" : "context-menu";
		const kind = actionPanelItemKind(item);

		if (!kind) {
			return;
		}

		if (kind === "separator") {
			const separator = document.createElement("div");
			separator.className = prefix + "-separator";
			container.appendChild(separator);
			return;
		}

		if (kind === "label") {
			const labelClass = variant === "menu" ? "menu-item-label" : "context-menu-label";
			const label = document.createElement("div");
			label.className = prefix + "-label-row";
			if (item.hint) {
				label.title = item.hint;
			}
			label.innerHTML = "<span class=\"" + labelClass + "\"></span>";
			label.querySelector("." + labelClass).textContent = item.label || "";
			container.appendChild(label);
			return;
		}

		if (kind === "slider") {
			const slider = document.createElement("div");
			slider.className = prefix + "-slider" + (item.tone ? " is-" + item.tone : "");
			if (item.hint) {
				slider.title = item.hint;
			}
			slider.innerHTML = "<div class=\"" + prefix + "-slider-header\"><span class=\"" + prefix
				+ "-slider-label\"></span><span class=\"" + prefix + "-slider-value\"></span></div><input class=\""
				+ prefix + "-slider-input\" type=\"range\">";

			const label = slider.querySelector("." + prefix + "-slider-label");
			const value = slider.querySelector("." + prefix + "-slider-value");
			const input = slider.querySelector("." + prefix + "-slider-input");
			label.textContent = item.label || "Value";
			value.textContent = sliderValueLabel(item, item.value);
			input.min = String(item.min);
			input.max = String(item.max);
			input.step = String(item.step || 1);
			input.value = String(item.value || 0);
			input.disabled = item.enabled === false;

			const emitValue = function(final) {
				value.textContent = sliderValueLabel(item, input.value);
				if (typeof item.onValueChange === "function") {
					item.onValueChange(Number(input.value), final);
				}
			};

			input.addEventListener("input", function(event) {
				event.stopPropagation();
				emitValue(item.finalOnRelease ? false : true);
			});
			if (item.finalOnRelease) {
				input.addEventListener("change", function(event) {
					event.stopPropagation();
					emitValue(true);
				});
			}
			input.addEventListener("pointerdown", function(event) {
				event.stopPropagation();
			});
			input.addEventListener("click", function(event) {
				event.stopPropagation();
			});
			slider.addEventListener("click", function(event) {
				event.stopPropagation();
			});

			container.appendChild(slider);
			return;
		}

		const button = document.createElement("button");
		button.type = "button";
		const labelClass = variant === "menu" ? "menu-item-label" : "context-menu-label";
		const stateClass = variant === "menu" ? "menu-item-state" : "context-menu-state";
		button.className = prefix + "-item"
			+ (item.checked ? " is-checked" : "")
			+ (item.tone ? " is-" + item.tone : "");
		button.disabled = item.enabled === false;
		if (item.hint) {
			button.title = item.hint;
		}
		button.innerHTML = "<span class=\"" + labelClass + "\"></span><span class=\"" + stateClass + "\"></span>";
		button.querySelector("." + labelClass).textContent = item.label || "Action";
		button.querySelector("." + stateClass).textContent = item.checked ? "On" : "";
		button.addEventListener("click", function(event) {
			event.stopPropagation();
			if (hideOnAction) {
				if (variant === "menu") {
					closeTopMenu();
				} else {
					hideContextMenu();
				}
			}
			if (item.enabled === false || typeof item.action !== "function") {
				return;
			}
			item.action();
		});
		container.appendChild(button);
	}

	function presenceStatusIconSvg(kind) {
		switch (String(kind || "")) {
			case "talking":
				return "<svg viewBox=\"0 0 24 24\" aria-hidden=\"true\"><path d=\"M12 5v14\"></path><path d=\"M7 9v6\"></path><path d=\"M17 9v6\"></path><path d=\"M4 11v2\"></path><path d=\"M20 11v2\"></path></svg>";
			case "whispering":
				return "<svg viewBox=\"0 0 24 24\" aria-hidden=\"true\"><path d=\"M11 5L6 9H3v6h3l5 4z\"></path><path d=\"M16 10.5a3 3 0 010 3\"></path></svg>";
			case "shouting":
				return "<svg viewBox=\"0 0 24 24\" aria-hidden=\"true\"><path d=\"M11 5L6 9H3v6h3l5 4z\"></path><path d=\"M15.5 8.5a5 5 0 010 7\"></path><path d=\"M18.5 5.5a9 9 0 010 13\"></path></svg>";
			case "mutedTalking":
				return "<svg viewBox=\"0 0 24 24\" aria-hidden=\"true\"><path d=\"M11 5L6 9H3v6h3l5 4z\"></path><path d=\"M16 9l5 5\"></path><path d=\"M21 9l-5 5\"></path></svg>";
			case "priority":
				return "<svg viewBox=\"0 0 24 24\" aria-hidden=\"true\"><path d=\"M12 3l2.1 5.9H20l-4.8 3.6 1.8 6.2L12 15l-5 3.7 1.8-6.2L4 8.9h5.9z\"></path></svg>";
			case "recording":
				return "<svg viewBox=\"0 0 24 24\" aria-hidden=\"true\"><circle cx=\"12\" cy=\"12\" r=\"6\"></circle><circle cx=\"12\" cy=\"12\" r=\"2.5\" fill=\"currentColor\" stroke=\"none\"></circle></svg>";
			case "friend":
				return "<svg viewBox=\"0 0 24 24\" aria-hidden=\"true\"><path d=\"M12 3.8l2.55 5.18 5.72.83-4.13 4.03.98 5.69L12 16.83l-5.12 2.7.98-5.69-4.13-4.03 5.72-.83z\"></path></svg>";
			case "authenticated":
				return "<svg viewBox=\"0 0 24 24\" aria-hidden=\"true\"><path d=\"M12 3l6 2.5V11c0 4-2.56 7.28-6 8.5-3.44-1.22-6-4.5-6-8.5V5.5z\"></path><path d=\"M9.3 11.8l1.8 1.8 3.6-3.9\"></path></svg>";
			case "listener":
				return "<svg viewBox=\"0 0 24 24\" aria-hidden=\"true\"><path d=\"M12 5a4 4 0 014 4v3.5\"></path><path d=\"M12 19a4.5 4.5 0 01-4.5-4.5V11a4.5 4.5 0 019 0\"></path><path d=\"M16.5 10.5a4.5 4.5 0 013 4\"></path></svg>";
			case "serverDeafened":
			case "selfDeafened":
				return "<svg viewBox=\"0 0 24 24\" aria-hidden=\"true\"><path d=\"M11 5L6 9H3v6h3l5 4z\"></path><path d=\"M16 9l5 5\"></path><path d=\"M21 9l-5 5\"></path></svg>";
			case "serverMuted":
			case "selfMuted":
			case "localMuted":
				return "<svg viewBox=\"0 0 24 24\" aria-hidden=\"true\"><path d=\"M11 5L6 9H3v6h3l5 4z\"></path><path d=\"M15.5 8.5a5 5 0 010 7\"></path></svg>";
			case "suppressed":
				return "<svg viewBox=\"0 0 24 24\" aria-hidden=\"true\"><path d=\"M5 12h14\"></path><path d=\"M8 8.5h8\"></path><path d=\"M8 15.5h8\"></path></svg>";
			default:
				return "<svg viewBox=\"0 0 24 24\" aria-hidden=\"true\"><circle cx=\"12\" cy=\"12\" r=\"5\"></circle></svg>";
		}
	}

	function normalizedTalkState(person) {
		return String(person && person.talkState || "passive").trim();
	}

	function talkStateClass(person) {
		switch (normalizedTalkState(person)) {
			case "talking":
				return " is-talking is-talk-talking";
			case "whispering":
				return " is-talking is-talk-whispering";
			case "shouting":
				return " is-talking is-talk-shouting";
			case "mutedTalking":
				return " is-talking is-talk-muted";
			default:
				return "";
		}
	}

	function renderPresenceStatuses(container, statuses) {
		const visibleStatuses = (statuses || []).filter(function(status) {
			return !!(status && status.kind);
		});

		container.innerHTML = "";
		container.classList.toggle("hidden", !visibleStatuses.length);
		visibleStatuses.forEach(function(status) {
			const indicator = document.createElement("span");
			indicator.className = "presence-status" + (status.tone ? " is-" + status.tone : "");
			indicator.title = status.label || "";
			indicator.setAttribute("aria-label", status.label || "");
			indicator.innerHTML = presenceStatusIconSvg(status.kind);
			container.appendChild(indicator);
		});
	}

	function presenceRowTitle(person) {
		const parts = [];
		const detailLabels = [];

		if (person && person.subtitle) {
			parts.push(person.subtitle);
		}

		(person && person.statuses ? person.statuses : []).forEach(function(status) {
			const label = status && status.label ? String(status.label) : "";
			if (label && detailLabels.indexOf(label) === -1) {
				detailLabels.push(label);
			}
		});

		(person && person.badges ? person.badges : []).forEach(function(badge) {
			const label = badge ? String(badge) : "";
			if (label && detailLabels.indexOf(label) === -1) {
				detailLabels.push(label);
			}
		});

		if (detailLabels.length) {
			parts.push(detailLabels.join(", "));
		}

		return parts.join(" | ");
	}

	function renderVoicePresenceStack(people) {
		refs.voicePresenceStack.innerHTML = "";
		(people || []).filter(function(person) {
			return String(person && person.entryKind || "user") !== "listener";
		}).slice(0, 5).forEach(function(person) {
			const avatar = document.createElement("div");
			avatar.className = "stack-avatar" + (person.isSelf ? " is-self" : "") + talkStateClass(person);
			styleAvatar(avatar, person.label, !!person.isSelf, person.avatarUrl || "");
			avatar.title = [person.label || "", person.talkLabel || ""].filter(Boolean).join(" | ");
			refs.voicePresenceStack.appendChild(avatar);
		});
	}

	function clearRoomDropTarget(event) {
		const target = event.currentTarget || event.target;
		if (!target || !target.classList) {
			return;
		}
		target.classList.remove("is-drop-target", "is-drop-before", "is-drop-after", "is-drop-inside");
		target.removeAttribute("data-drop-placement");
	}

	function clearAllRoomDropTargets() {
		document.querySelectorAll(".rail-row.is-drop-target, .rail-root-label.is-drop-target").forEach(function(element) {
			element.classList.remove("is-drop-target", "is-drop-before", "is-drop-after", "is-drop-inside");
			element.removeAttribute("data-drop-placement");
		});
	}

	function roomDropPlacement(event, target) {
		if (!dragState || dragState.kind !== "room" || !target || target.dataset.dropTarget === "root") {
			return "inside";
		}

		const bounds = target.getBoundingClientRect();
		if (!bounds.height) {
			return "inside";
		}

		const y = Math.max(0, Math.min(bounds.height, event.clientY - bounds.top));
		if (y <= bounds.height * 0.30) {
			return "before";
		}
		if (y >= bounds.height * 0.70) {
			return "after";
		}
		return "inside";
	}

	function applyRoomDropTarget(target, placement) {
		if (!target || !target.classList) {
			return;
		}

		target.classList.remove("is-drop-before", "is-drop-after", "is-drop-inside");
		target.classList.add("is-drop-target", "is-drop-" + (placement || "inside"));
		target.dataset.dropPlacement = placement || "inside";
	}

	function handleRoomDragEnter(event) {
		const target = event.currentTarget;
		if (!target || !dragState || !target.classList) {
			return;
		}

		if (target.dataset.dropTarget === "root" && dragState.kind !== "room") {
			return;
		}

		if (dragState.kind === "participant" || dragState.kind === "room") {
			event.preventDefault();
			applyRoomDropTarget(target, roomDropPlacement(event, target));
		}
	}

	function handleRoomDragOver(event) {
		const target = event.currentTarget;
		if (!target || !dragState || !target.classList) {
			return;
		}

		if (target.dataset.dropTarget === "root" && dragState.kind !== "room") {
			return;
		}

		if (dragState.kind === "participant" || dragState.kind === "room") {
			event.preventDefault();
			if (event.dataTransfer) {
				event.dataTransfer.dropEffect = "move";
			}
			applyRoomDropTarget(target, roomDropPlacement(event, target));
		}
	}

	function participantContextMenuItems(participant, scopeToken) {
		const items = [];
		if (!participant) {
			return items;
		}

		if (participant.entryKind !== "listener" && participant.canMessage) {
			items.push({
				label: "Open message",
				enabled: true,
				action: function() {
					notifyBridge("messageParticipant", participant.session);
				}
			});
		}
		if (participant.entryKind !== "listener" && participant.canJoin) {
			items.push({
				label: "Join room",
				enabled: true,
				action: function() {
					notifyBridge("joinParticipant", participant.session);
				}
			});
		}

		const participantActionItems = actionItemsFromActionStates(participant.actions, participant.entryKind === "listener"
			? {
				invokeAction: function(actionId) {
					notifyBridge("invokeScopeAction", participant.scopeToken || scopeToken, actionId);
				},
				invokeValueChanged: function(actionId, value, final) {
					notifyBridge("scopeActionValueChanged",
						participant.scopeToken || scopeToken,
						actionId,
						value,
						final);
				}
			}
			: {
				invokeAction: function(actionId) {
					notifyBridge("invokeParticipantAction", participant.session, actionId);
				},
				invokeValueChanged: function(actionId, value, final) {
					notifyBridge("participantActionValueChanged", participant.session, actionId, value, final);
				}
			});
		if (items.length && participantActionItems.length) {
			items.push({ separator: true });
		}
		return items.concat(participantActionItems);
	}

	function renderPresenceList(container, room, people) {
		const list = document.createElement("div");
		list.className = "presence-list";

		(people || []).forEach(function(person) {
			const entry = document.createElement("div");
			entry.className = "presence-entry";

			const row = document.createElement("button");
			row.type = "button";
			row.className = "presence-row"
				+ (person.isSelf ? " is-self" : "")
				+ (person.entryKind === "listener" ? " is-listener" : "")
				+ talkStateClass(person);
			row.dataset.session = person.session ? String(person.session) : "";
			row.dataset.participantKey = participantStateKey(person);
			row.dataset.entryKind = String(person.entryKind || "user");
			row.dataset.canMessage = person.canMessage ? "true" : "false";
			row.dataset.canJoin = person.canJoin ? "true" : "false";
			row.dataset.roomToken = room && room.token ? String(room.token) : "";
			row.dataset.scopeToken = person.scopeToken ? String(person.scopeToken) : (room && room.token ? String(room.token) : "");
			row.draggable = person.entryKind !== "listener" && !!row.dataset.session;
			row.classList.toggle("is-disabled",
				!person.canMessage && !person.canJoin && !((person.actions || []).length));
			row.title = presenceRowTitle(person);

			const avatar = document.createElement("span");
			avatar.className = "presence-avatar"
				+ (person.isSelf ? " is-self" : "")
				+ (person.entryKind === "listener" ? " is-listener" : "")
				+ talkStateClass(person);
			styleAvatar(avatar, person.label, !!person.isSelf, person.avatarUrl || "");

			const dot = document.createElement("span");
			dot.className = "presence-dot" + (person.entryKind === "listener" ? " is-listener" : "")
				+ talkStateClass(person);

			const label = document.createElement("span");
			label.className = "presence-copy";
			label.innerHTML = "<span class=\"presence-heading\"><span class=\"presence-name\"></span><span class=\"presence-statuses hidden\"></span></span><span class=\"presence-subtitle\"></span>";
			label.querySelector(".presence-name").textContent = person.label || "Unknown";
			renderPresenceStatuses(label.querySelector(".presence-statuses"), person.statuses || []);
			label.querySelector(".presence-subtitle").textContent = person.subtitle || "";
			label.querySelector(".presence-subtitle").classList.toggle("hidden", !(person.subtitle || ""));

			row.appendChild(avatar);
			row.appendChild(dot);
			row.appendChild(label);
			entry.appendChild(row);

			if ((person.actions || []).length) {
				const actionsButton = document.createElement("button");
				actionsButton.type = "button";
				actionsButton.className = "presence-action-button";
				actionsButton.title = "More actions";
				actionsButton.setAttribute("aria-label", "More actions for " + (person.label || "participant"));
				actionsButton.innerHTML = "<svg viewBox=\"0 0 24 24\" aria-hidden=\"true\"><circle cx=\"5\" cy=\"12\" r=\"1.8\"></circle><circle cx=\"12\" cy=\"12\" r=\"1.8\"></circle><circle cx=\"19\" cy=\"12\" r=\"1.8\"></circle></svg>";
				actionsButton.addEventListener("click", function(event) {
					event.stopPropagation();
					event.preventDefault();
					const bounds = actionsButton.getBoundingClientRect();
					showContextMenu(participantContextMenuItems(person, row.dataset.scopeToken),
						Math.max(8, bounds.right - 4),
						Math.max(8, bounds.bottom + 4));
				});
				entry.appendChild(actionsButton);
			}

			row.addEventListener("click", function(event) {
				event.stopPropagation();
				if (row.dataset.entryKind === "listener") {
					return;
				}
				if (row.dataset.canMessage === "true") {
					notifyBridge("messageParticipant", row.dataset.session);
				} else if (row.dataset.canJoin === "true") {
					notifyBridge("joinParticipant", row.dataset.session);
				}
				dismissCompactRailAfterAction();
			});
			row.addEventListener("dragstart", function(event) {
				if (!row.dataset.session || row.dataset.entryKind === "listener") {
					event.preventDefault();
					return;
				}

				dragState = {
					kind: "participant",
					session: row.dataset.session,
					sourceRoomToken: row.dataset.roomToken || ""
				};
				row.classList.add("is-dragging");
				if (event.dataTransfer) {
					event.dataTransfer.effectAllowed = "move";
					event.dataTransfer.setData("text/plain", "participant:" + row.dataset.session);
				}
			});
			row.addEventListener("dragend", function() {
				row.classList.remove("is-dragging");
				dragState = null;
				clearAllRoomDropTargets();
			});
			list.appendChild(entry);
		});

		container.appendChild(list);
	}

	function buildRoomRow(room, joinable, selectedVoicePresence) {
		const depthValue = Number(room.depth);
		const depth = Number.isFinite(depthValue) && depthValue > 0 ? depthValue : 0;
		const roomPathLabel = room.pathLabel || "";
		const unreadCount = Number(room.unreadCount || 0);
		const memberCount = Number(room.memberCount || 0);
		const screenShare = room.screenShare || {};
		const roomKind = String(room.kindLabel || "").trim().toLowerCase();
		const hasRoomActions = (room.actions || []).some(function(item) {
			return !!item && String(item.kind || "action") !== "separator";
		});
		const subtitleText = joinable
			? ((room.joined || room.selected) ? (roomPathLabel || room.description || "") : "")
			: ((room.selected || unreadCount > 0 || roomKind === "activity" || roomKind === "direct message")
				? (room.description || "")
				: "");
		const wrapper = document.createElement("div");
		wrapper.className = "rail-row-wrapper" + (joinable ? " is-voice-room" : "");
		wrapper.style.setProperty("--room-depth", String(depth));

		const button = document.createElement("button");
		button.type = "button";
		button.className = "rail-row"
			+ (room.selected ? " is-selected" : "")
			+ (room.joined ? " is-joined" : "");
		button.classList.toggle("has-subtitle", !!subtitleText);
		button.classList.toggle("has-unread", unreadCount > 0);
		button.classList.toggle("is-populated", joinable && memberCount > 0);
		button.dataset.scopeToken = room.token || "";
		button.dataset.canJoin = joinable && !room.joined ? "true" : "false";
		button.dataset.roomLabel = room.label || "";
		button.dataset.depth = String(depth);
		button.dataset.roomType = joinable ? "voice" : "text";
		button.draggable = !!joinable;
		if (room.participantSession) {
			button.dataset.participantSession = String(room.participantSession);
		}
		button.title = roomPathLabel || room.description || "";

		const chip = document.createElement("span");
		chip.className = "kind-chip";
		chip.textContent = kindChipText(room.kindLabel);

		const copy = document.createElement("span");
		copy.className = "rail-row-copy";

		const title = document.createElement("span");
		title.className = "rail-row-title";
		title.textContent = room.label || "Room";

		const subtitle = document.createElement("span");
		subtitle.className = "rail-row-subtitle";
		subtitle.textContent = subtitleText;
		subtitle.classList.toggle("hidden", !subtitleText);

		copy.appendChild(title);
		copy.appendChild(subtitle);

		const meta = document.createElement("span");
		meta.className = "rail-row-meta";

		if (unreadCount > 0) {
			const unread = document.createElement("span");
			unread.className = "row-badge";
			unread.textContent = String(unreadCount);
			meta.appendChild(unread);
		}

		if (joinable && memberCount > 0) {
			const count = document.createElement("span");
			count.className = "row-count";
			count.classList.toggle("is-active-room", !!room.joined);
			count.textContent = String(memberCount);
			count.title = memberCount === 1 ? "1 person in room" : (String(memberCount) + " people in room");
			meta.appendChild(count);
		}

		if (joinable && screenShareVisible(screenShare) && screenShare.badgeLabel) {
			const shareBadge = document.createElement("span");
			shareBadge.className = "meta-pill row-share-badge"
				+ (screenShare.badgeTone ? " is-" + screenShare.badgeTone : "");
			shareBadge.textContent = screenShare.badgeLabel;
			shareBadge.title = screenShare.statusLabel || screenShare.badgeLabel;
			meta.appendChild(shareBadge);
		}

		if (joinable) {
			const joinButton = document.createElement("button");
			joinButton.type = "button";
			joinButton.className = "mini-action";
			joinButton.textContent = room.joined ? "Live" : "Join";
			joinButton.disabled = !!room.joined;
			joinButton.addEventListener("click", function(event) {
				event.stopPropagation();
				notifyBridge("joinVoiceChannel", room.token);
				dismissCompactRailAfterAction();
			});
			meta.appendChild(joinButton);
		}

		if (joinable && room.joined && screenShareVisible(screenShare) && screenShare.primaryActionId) {
			const shareActionButton = document.createElement("button");
			shareActionButton.type = "button";
			shareActionButton.className = "mini-action room-share-action"
				+ (screenShare.primaryTone ? " is-" + screenShare.primaryTone : "");
			shareActionButton.textContent = compactScreenShareActionLabel(screenShare.primaryLabel);
			shareActionButton.disabled = screenShare.primaryEnabled === false;
			shareActionButton.title = screenShare.primaryHint || screenShare.primaryLabel || "Screen share";
			shareActionButton.addEventListener("click", function(event) {
				event.stopPropagation();
				notifyBridge("invokeScopeAction", room.token, screenShare.primaryActionId);
				dismissCompactRailAfterAction();
			});
			meta.appendChild(shareActionButton);
		}

		if (hasRoomActions) {
			const roomActionButton = document.createElement("button");
			roomActionButton.type = "button";
			roomActionButton.className = "room-action-button";
			roomActionButton.title = "Room actions";
			roomActionButton.setAttribute("aria-label", "Room actions for " + (room.label || "room"));
			roomActionButton.innerHTML = "<svg viewBox=\"0 0 24 24\" aria-hidden=\"true\"><circle cx=\"5\" cy=\"12\" r=\"1.8\"></circle><circle cx=\"12\" cy=\"12\" r=\"1.8\"></circle><circle cx=\"19\" cy=\"12\" r=\"1.8\"></circle></svg>";
			roomActionButton.addEventListener("click", function(event) {
				event.preventDefault();
				event.stopPropagation();
				const items = buildRoomContextMenuItems(getSnapshot(), room, button).filter(function(item) {
					return !!item;
				});
				const bounds = roomActionButton.getBoundingClientRect();
				showContextMenu(items, bounds.right, bounds.bottom + 4);
			});
			meta.appendChild(roomActionButton);
		}

		button.appendChild(chip);
		button.appendChild(copy);
		button.appendChild(meta);
		button.addEventListener("click", function() {
			notifyBridge("selectScope", room.token);
			dismissCompactRailAfterAction();
		});
		button.addEventListener("dblclick", function(event) {
			if (!joinable) {
				return;
			}
			event.preventDefault();
			event.stopPropagation();
			notifyBridge("joinVoiceChannel", room.token);
			dismissCompactRailAfterAction();
		});
		button.addEventListener("dragstart", function(event) {
			if (!joinable || !room.token) {
				event.preventDefault();
				return;
			}

			dragState = {
				kind: "room",
				scopeToken: room.token
			};
			button.classList.add("is-dragging");
			if (event.dataTransfer) {
				event.dataTransfer.effectAllowed = "move";
				event.dataTransfer.setData("text/plain", "room:" + room.token);
			}
		});
		button.addEventListener("dragend", function() {
			button.classList.remove("is-dragging");
			dragState = null;
			clearAllRoomDropTargets();
		});
		button.addEventListener("dragenter", handleRoomDragEnter);
		button.addEventListener("dragover", handleRoomDragOver);
		button.addEventListener("dragleave", clearRoomDropTarget);
		button.addEventListener("drop", function(event) {
			clearRoomDropTarget(event);
			if (!joinable || !dragState) {
				return;
			}

			event.preventDefault();
			if (dragState.kind === "participant" && dragState.session) {
				notifyBridge("moveParticipantToChannel", dragState.session, room.token);
			} else if (dragState.kind === "room" && dragState.scopeToken && dragState.scopeToken !== room.token) {
				notifyBridge("moveChannelToChannel",
					dragState.scopeToken,
					room.token,
					button.dataset.dropPlacement || roomDropPlacement(event, button));
			}
		});

		wrapper.appendChild(button);

		const roomParticipants = joinable
			? ((room.participants && room.participants.length) ? room.participants : ((room.joined && selectedVoicePresence) || []))
			: [];
		if (roomParticipants.length) {
			renderPresenceList(wrapper, room, roomParticipants);
		}

		return wrapper;
	}

	function renderRoomList(container, rooms, options) {
		const roomSection = container.closest(".room-card-block");
		const roomList = rooms || [];
		const hasRooms = roomList.length > 0;
		if (roomSection) {
			roomSection.classList.toggle("is-empty", !hasRooms);
			roomSection.classList.toggle("hidden", !!options.hideWhenEmpty && !hasRooms);
		}

		container.innerHTML = "";

		if (!hasRooms) {
			if (options.hideWhenEmpty) {
				return;
			}

			const empty = document.createElement("div");
			empty.className = "rail-empty";
			empty.textContent = options.emptyText || "Waiting for room state.";
			container.appendChild(empty);
			return;
		}

		if (options.rootLabel) {
			const rootLabel = document.createElement("div");
			rootLabel.className = "rail-root-label";
			rootLabel.textContent = options.rootLabel;
			rootLabel.dataset.dropTarget = "root";
			rootLabel.dataset.scopeToken = options.rootToken || "";
			if (options.joinable && options.rootToken) {
				rootLabel.addEventListener("dragenter", handleRoomDragEnter);
				rootLabel.addEventListener("dragover", handleRoomDragOver);
				rootLabel.addEventListener("dragleave", clearRoomDropTarget);
				rootLabel.addEventListener("drop", function(event) {
					clearRoomDropTarget(event);
					if (!dragState || dragState.kind !== "room" || !dragState.scopeToken) {
						return;
					}

					event.preventDefault();
					notifyBridge("moveChannelToChannel", dragState.scopeToken, options.rootToken, "inside");
				});
			}
			container.appendChild(rootLabel);
		}

		roomList.forEach(function(room) {
			container.appendChild(buildRoomRow(room, options.joinable, options.voicePresence));
		});
	}

	function composerCanAttachImages() {
		const scope = getSnapshot().activeScope || {};
		return !!scope.canAttachImages && !refs.composerInput.disabled;
	}

	function isImageFile(file) {
		if (!file) {
			return false;
		}

		const mime = String(file.type || "").toLowerCase();
		if (mime.indexOf("image/") === 0) {
			return true;
		}

		return /\.(png|jpe?g|gif|webp|bmp)$/i.test(String(file.name || ""));
	}

	function imageFilesFromList(fileList) {
		return Array.from(fileList || []).filter(isImageFile);
	}

	function imageFilesFromDataTransfer(dataTransfer) {
		if (!dataTransfer) {
			return [];
		}

		const directFiles = imageFilesFromList(dataTransfer.files);
		if (directFiles.length) {
			return directFiles;
		}

		return Array.from(dataTransfer.items || []).map(function(item) {
			if (!item || item.kind !== "file" || String(item.type || "").toLowerCase().indexOf("image/") !== 0) {
				return null;
			}
			return typeof item.getAsFile === "function" ? item.getAsFile() : null;
		}).filter(isImageFile);
	}

	function hasImageFileTransfer(dataTransfer) {
		return imageFilesFromDataTransfer(dataTransfer).length > 0;
	}

	function clipboardTransferLooksImageLike(dataTransfer) {
		if (!dataTransfer) {
			return false;
		}

		if (hasImageFileTransfer(dataTransfer)) {
			return true;
		}

		return Array.from(dataTransfer.types || []).some(function(type) {
			const normalizedType = String(type || "").toLowerCase();
			return normalizedType === "files"
				|| normalizedType === "text/uri-list"
				|| normalizedType === "application/x-qt-image"
				|| normalizedType.indexOf("image/") === 0;
		});
	}

	function readFileAsDataUrl(file) {
		return new Promise(function(resolve, reject) {
			const reader = new FileReader();
			reader.onload = function() {
				resolve(String(reader.result || ""));
			};
			reader.onerror = function() {
				reject(reader.error || new Error("Unable to read image file"));
			};
			reader.readAsDataURL(file);
		});
	}

	function attachImageDataUrl(dataUrl) {
		const normalizedDataUrl = String(dataUrl || "");
		if (!normalizedDataUrl || normalizedDataUrl.indexOf("data:image/") !== 0) {
			return;
		}

		notifyBridge("attachImageData", normalizedDataUrl);
	}

	function attachImageFiles(files) {
		imageFilesFromList(files).forEach(function(file) {
			readFileAsDataUrl(file).then(function(dataUrl) {
				attachImageDataUrl(dataUrl);
			}).catch(function(error) {
				console.warn("Unable to attach image file:", error);
			});
		});
	}

	function setComposerImageDropTarget(active) {
		refs.composerShell.classList.toggle("is-image-drop-target", !!active);
	}

	function handleComposerImagePaste(event) {
		if (!composerCanAttachImages()) {
			return;
		}

		if (!clipboardTransferLooksImageLike(event.clipboardData)) {
			return;
		}

		event.preventDefault();
		notifyBridge("attachClipboardImage");
	}

	function handleComposerImageDragEnter(event) {
		if (!composerCanAttachImages() || dragState || !hasImageFileTransfer(event.dataTransfer)) {
			return;
		}

		event.preventDefault();
		setComposerImageDropTarget(true);
	}

	function handleComposerImageDragOver(event) {
		if (!composerCanAttachImages() || dragState || !hasImageFileTransfer(event.dataTransfer)) {
			return;
		}

		event.preventDefault();
		if (event.dataTransfer) {
			event.dataTransfer.dropEffect = "copy";
		}
		setComposerImageDropTarget(true);
	}

	function clearComposerImageDropTarget() {
		setComposerImageDropTarget(false);
	}

	function bridgeClipboardHasImage(callback) {
		if (!modernBridge || typeof modernBridge.clipboardHasImage !== "function") {
			callback(false);
			return;
		}

		try {
			modernBridge.clipboardHasImage(function(result) {
				callback(!!result);
			});
		} catch (error) {
			console.warn("Unable to query clipboard image state:", error);
			callback(false);
		}
	}

	function bridgeClipboardText(callback) {
		if (!modernBridge || typeof modernBridge.clipboardText !== "function") {
			callback("");
			return;
		}

		try {
			modernBridge.clipboardText(function(text) {
				callback(String(text || ""));
			});
		} catch (error) {
			console.warn("Unable to read clipboard text:", error);
			callback("");
		}
	}

	function pastePlainTextIntoComposer() {
		bridgeClipboardText(function(text) {
			if (text) {
				replaceComposerSelection(text);
			}
		});
		if (modernBridge && typeof modernBridge.clipboardText === "function") {
			return;
		}

		try {
			if (typeof document.execCommand === "function" && document.execCommand("paste")) {
				return;
			}
		} catch (error) {}

		if (navigator.clipboard && typeof navigator.clipboard.readText === "function") {
			navigator.clipboard.readText().then(function(text) {
				if (text) {
					replaceComposerSelection(text);
				}
			}).catch(function() {});
		} else {
			document.execCommand("paste");
		}
	}

	function handleComposerImageDrop(event) {
		if (!composerCanAttachImages() || dragState || !hasImageFileTransfer(event.dataTransfer)) {
			return;
		}

		event.preventDefault();
		clearComposerImageDropTarget();
		attachImageFiles(imageFilesFromDataTransfer(event.dataTransfer));
		refs.composerInput.focus();
	}

	function sendComposerDraft() {
		const value = refs.composerInput.value.trim();
		if (!value) {
			return false;
		}

		notifyBridge("sendMessage", value);
		refs.composerInput.value = "";
		syncComposerHeight();
		return true;
	}

	function handleComposerInputKeyDown(event) {
		const key = String(event.key || "");
		if ((key === "Enter" || key === "Return") && !event.shiftKey && !event.isComposing && event.keyCode !== 229) {
			event.preventDefault();
			sendComposerDraft();
			return;
		}

		if (!composerCanAttachImages()) {
			return;
		}

		const isPasteShortcut = (event.ctrlKey || event.metaKey) && !event.altKey && !event.shiftKey
			&& String(event.key || "").toLowerCase() === "v";
		if (!isPasteShortcut) {
			return;
		}

		event.preventDefault();
		bridgeClipboardHasImage(function(hasImage) {
			if (hasImage) {
				notifyBridge("attachClipboardImage");
				return;
			}

			pastePlainTextIntoComposer();
		});
	}

	function renderSystemMessage(message) {
		const article = document.createElement("article");
		article.className = "system-message";
		article.innerHTML =
			"<span class=\"system-label\"></span><span class=\"system-time\"></span><div class=\"system-body\"></div>";
		article.querySelector(".system-label").textContent = message.actor || "System";
		article.querySelector(".system-time").textContent = message.timeLabel || "";
		article.querySelector(".system-body").innerHTML = message.bodyHtml || escapeHtml(message.bodyText || "");
		return article;
	}

	function appendReplyBlock(container, message) {
		if (!message.replyActor && !message.replySnippet) {
			return;
		}

		const reply = document.createElement("div");
		reply.className = "reply-block";
		reply.innerHTML =
			"<div class=\"reply-actor\"></div><div class=\"reply-snippet\"></div>";
		reply.querySelector(".reply-actor").textContent = message.replyActor || "Reply";
		reply.querySelector(".reply-snippet").textContent = message.replySnippet || "";
		container.appendChild(reply);
	}

	function reactionStateForEmoji(message, emoji) {
		const reactions = (message && message.reactions) || [];
		for (let index = 0; index < reactions.length; index += 1) {
			if (String(reactions[index].emoji || "") === String(emoji || "")) {
				return reactions[index];
			}
		}
		return null;
	}

	function previewHostLabel(url) {
		const rawUrl = String(url || "").trim();
		if (!rawUrl) {
			return "";
		}

		try {
			return String(new URL(rawUrl).hostname || "").replace(/^www\./i, "");
		} catch (error) {
			return "";
		}
	}

	function previewSourceLabel(preview, hostLabel) {
		const subtitle = String((preview && preview.subtitle) || "").trim();
		if (subtitle) {
			return subtitle;
		}
		if (hostLabel) {
			return hostLabel;
		}
		return "Link preview";
	}

	function previewDescriptionText(preview) {
		const description = String((preview && preview.description) || "").trim();
		if (description) {
			return description;
		}
		if (preview && preview.loading) {
			return "Loading preview...";
		}
		if (preview && preview.failed) {
			return "Preview unavailable";
		}
		return "";
	}

	function previewBadgeText(preview, sourceLabel, hostLabel) {
		if (hostLabel && hostLabel.toLowerCase() !== sourceLabel.toLowerCase()) {
			return hostLabel;
		}
		if (preview && preview.loading) {
			return "Loading";
		}
		if (preview && preview.failed) {
			return "Limited";
		}
		return "";
	}

	function previewPlaceholderMark(preview, hostLabel) {
		const fallbackSource = hostLabel
			|| String((preview && preview.subtitle) || "").trim()
			|| String((preview && preview.title) || "").trim()
			|| "Link";
		const simplified = fallbackSource
			.replace(/^www\./i, "")
			.replace(/\.[a-z0-9-]+$/i, "")
			.replace(/[^a-z0-9]+/ig, " ")
			.trim();
		const tokens = simplified ? simplified.split(/\s+/).filter(Boolean) : [];
		if (!tokens.length) {
			return preview && preview.kind === "image" ? "IMG" : "LN";
		}
		if (tokens.length === 1) {
			return tokens[0].slice(0, Math.min(2, tokens[0].length)).toUpperCase();
		}
		return (tokens[0][0] + tokens[1][0]).toUpperCase();
	}

	function renderPreviewCard(message) {
		const preview = message && message.preview;
		if (!preview || (!preview.url && !preview.title && !preview.description && !preview.thumbnailUrl)) {
			return null;
		}
		const hasThumbnail = !!preview.thumbnailUrl;
		const hostLabel = previewHostLabel(preview.url);
		const sourceLabel = previewSourceLabel(preview, hostLabel);
		const badgeText = previewBadgeText(preview, sourceLabel, hostLabel);
		const descriptionText = previewDescriptionText(preview);

		const card = document.createElement("button");
		card.type = "button";
		card.className = "preview-card"
			+ (hasThumbnail ? " has-thumbnail" : "")
			+ (preview.loading ? " is-loading" : "")
			+ (preview.failed ? " is-failed" : "")
			+ (preview.kind === "image" ? " is-image" : "");
		card.title = preview.openLabel || "Open link";
		card.setAttribute("aria-label", preview.openLabel || "Open link");
		card.addEventListener("click", function(event) {
			event.preventDefault();
			event.stopPropagation();
			if (preview.url) {
				notifyBridge("activateLink", preview.url);
			}
		});

		if (preview.thumbnailUrl) {
			const media = document.createElement("div");
			media.className = "preview-card-media";
			const image = document.createElement("img");
			image.className = "preview-card-image";
			image.loading = "lazy";
			image.src = preview.thumbnailUrl;
			image.alt = preview.title || preview.subtitle || "Preview";
			media.appendChild(image);
			card.appendChild(media);
		} else {
			const placeholder = document.createElement("div");
			placeholder.className = "preview-card-media preview-card-media-placeholder";
			const placeholderMark = document.createElement("span");
			placeholderMark.className = "preview-card-placeholder-mark";
			placeholderMark.textContent = previewPlaceholderMark(preview, hostLabel);
			placeholder.appendChild(placeholderMark);

			const placeholderLabel = document.createElement("span");
			placeholderLabel.className = "preview-card-placeholder-label";
			placeholderLabel.textContent = preview.loading
				? "Loading preview"
				: (preview.failed ? "No image available" : "Link preview");
			placeholder.appendChild(placeholderLabel);
			card.appendChild(placeholder);
		}

		const copy = document.createElement("div");
		copy.className = "preview-card-copy";

		const meta = document.createElement("div");
		meta.className = "preview-card-meta";
		if (sourceLabel) {
			const subtitle = document.createElement("div");
			subtitle.className = "preview-card-subtitle";
			subtitle.textContent = sourceLabel;
			meta.appendChild(subtitle);
		}
		if (badgeText) {
			const badge = document.createElement("div");
			badge.className = "preview-card-badge";
			badge.textContent = badgeText;
			meta.appendChild(badge);
		}
		if (meta.childNodes.length) {
			copy.appendChild(meta);
		}

		const title = document.createElement("div");
		title.className = "preview-card-title";
		title.textContent = preview.title || preview.url || "Link preview";
		copy.appendChild(title);

		if (descriptionText) {
			const description = document.createElement("div");
			description.className = "preview-card-description";
			description.textContent = descriptionText;
			copy.appendChild(description);
		}

		const footer = document.createElement("div");
		footer.className = "preview-card-footer";
		const action = document.createElement("span");
		action.className = "preview-card-action";
		action.textContent = preview.openLabel || "Open link";
		footer.appendChild(action);
		copy.appendChild(footer);

		card.appendChild(copy);
		return card;
	}

	function renderReactionChip(message, reaction) {
		const button = document.createElement("button");
		button.type = "button";
		button.className = "reaction-chip" + (reaction.selfReacted ? " is-self" : "");
		button.innerHTML = "<span class=\"reaction-chip-emoji\"></span><span class=\"reaction-chip-count\"></span>";
		button.querySelector(".reaction-chip-emoji").textContent = reaction.emoji || "";
		button.querySelector(".reaction-chip-count").textContent = String(reaction.count || 0);
		button.addEventListener("click", function(event) {
			event.preventDefault();
			event.stopPropagation();
			openReactionPickerMessageId = null;
			notifyBridge("toggleReaction", message.messageId, reaction.emoji || "", !reaction.selfReacted);
		});
		return button;
	}

	function renderReactionPicker(message) {
		if (!message || !message.canReact || openReactionPickerMessageId !== message.messageId) {
			return null;
		}

		const picker = document.createElement("div");
		picker.className = "reaction-picker";
		starterReactionEmoji.forEach(function(emoji) {
			const reaction = reactionStateForEmoji(message, emoji);
			const button = document.createElement("button");
			button.type = "button";
			button.className = "reaction-picker-button" + (reaction && reaction.selfReacted ? " is-self" : "");
			button.textContent = emoji;
			button.title = reaction && reaction.selfReacted ? "Remove reaction" : "Add reaction";
			button.addEventListener("click", function(event) {
				event.preventDefault();
				event.stopPropagation();
				openReactionPickerMessageId = null;
				notifyBridge("toggleReaction", message.messageId, emoji, !(reaction && reaction.selfReacted));
			});
			picker.appendChild(button);
		});
		return picker;
	}

	function requestDeleteMessage(message) {
		if (!message || !message.canDelete) {
			return;
		}

		openReactionPickerMessageId = null;
		notifyBridge("deleteMessage", message.messageId);
	}

	function renderMessageFooter(message) {
		const reactions = (message && message.reactions) || [];
		const canReply = !!(message && message.canReply);
		const canReact = !!(message && message.canReact);
		const canDelete = !!(message && message.canDelete);
		if (!reactions.length && !canReply && !canReact && !canDelete) {
			return null;
		}

		const footer = document.createElement("div");
		footer.className = "bubble-footer";

		if (reactions.length) {
			const reactionRow = document.createElement("div");
			reactionRow.className = "reaction-row";
			reactions.forEach(function(reaction) {
				reactionRow.appendChild(renderReactionChip(message, reaction));
			});
			footer.appendChild(reactionRow);
		}

		if (canReply || canReact || canDelete) {
			const toolbar = document.createElement("div");
			toolbar.className = "bubble-toolbar";

			if (canReply) {
				const replyButton = document.createElement("button");
				replyButton.type = "button";
				replyButton.className = "bubble-toolbar-button";
				replyButton.textContent = "Reply";
				replyButton.addEventListener("click", function(event) {
					event.preventDefault();
					event.stopPropagation();
					openReactionPickerMessageId = null;
					notifyBridge("startReply", message.messageId);
				});
				toolbar.appendChild(replyButton);
			}

			if (canReact) {
				const reactionButton = document.createElement("button");
				reactionButton.type = "button";
				reactionButton.className = "icon-button bubble-toolbar-button bubble-toolbar-icon reaction-picker-toggle"
					+ (openReactionPickerMessageId === message.messageId ? " is-active" : "");
				reactionButton.innerHTML =
					"<svg viewBox=\"0 0 24 24\" aria-hidden=\"true\"><path d=\"M22 11v1a10 10 0 1 1-9-10\"></path><path d=\"M8 14s1.5 2 4 2 4-2 4-2\"></path><path d=\"M9 9h.01\"></path><path d=\"M15 9h.01\"></path><path d=\"M16 5h6\"></path><path d=\"M19 2v6\"></path></svg>";
				reactionButton.title = "Add reaction";
				reactionButton.setAttribute("aria-label", "Add reaction");
				reactionButton.addEventListener("click", function(event) {
					event.preventDefault();
					event.stopPropagation();
					const willOpen = openReactionPickerMessageId !== message.messageId;
					openReactionPickerMessageId = willOpen ? message.messageId : null;
					if (willOpen) {
						pauseReactionPickerScrollClose();
					} else {
						reactionPickerScrollClosePausedUntil = 0;
					}
					syncSnapshot();
				});
				toolbar.appendChild(reactionButton);
			}

			if (canDelete) {
				const deleteButton = document.createElement("button");
				deleteButton.type = "button";
				deleteButton.className = "bubble-toolbar-button is-danger";
				deleteButton.textContent = "Delete";
				deleteButton.addEventListener("click", function(event) {
					event.preventDefault();
					event.stopPropagation();
					requestDeleteMessage(message);
				});
				toolbar.appendChild(deleteButton);
			}

			footer.appendChild(toolbar);
		}

		const picker = renderReactionPicker(message);
		if (picker) {
			footer.appendChild(picker);
		}

		return footer;
	}

	function renderMessageBubble(message) {
		const bubble = document.createElement("div");
		bubble.className = "message-bubble" + (message.own ? " is-own" : "");
		bubble.dataset.bodyText = message.bodyText || "";
		bubble.dataset.messageId = String(message.messageId || "");
		appendReplyBlock(bubble, message);

		const body = document.createElement("div");
		body.className = "bubble-copy";
		body.innerHTML = message.bodyHtml || escapeHtml(message.bodyText || "");
		bubble.appendChild(body);

		const previewCard = renderPreviewCard(message);
		if (previewCard) {
			bubble.appendChild(previewCard);
		}

		const footer = renderMessageFooter(message);
		if (footer) {
			bubble.appendChild(footer);
		}
		return bubble;
	}

	function messageKey(message) {
		if (!message) {
			return "";
		}

		if (message.messageId) {
			return "id:" + String(message.messageId);
		}

		return [
			String(message.createdAtMs || ""),
			String(message.actor || ""),
			String(message.bodyText || ""),
			message.system ? "system" : "message"
		].join("|");
	}

	function latestTailMessageKey(messages) {
		for (let index = (messages || []).length - 1; index >= 0; index -= 1) {
			const key = messageKey(messages[index]);
			if (key) {
				return key;
			}
		}

		return "";
	}

	function countFreshTailMessages(messages, previousTailKey) {
		if (!previousTailKey) {
			return 0;
		}

		let count = 0;
		for (let index = (messages || []).length - 1; index >= 0; index -= 1) {
			const message = messages[index];
			if (messageKey(message) === previousTailKey) {
				break;
			}
			if (!message.system) {
				count += 1;
			}
		}

		return count;
	}

	function renderMessageGroups(messages) {
		const groups = [];
		let currentGroup = null;
		let previousMessage = null;

		(messages || []).forEach(function(message) {
			const dayLabel = dayLabelFromMs(message.createdAtMs);
			const needsDayDivider = !previousMessage || dayLabelFromMs(previousMessage.createdAtMs) !== dayLabel;

			if (needsDayDivider && dayLabel) {
				groups.push({ type: "day", label: dayLabel });
			}

			if (message.system) {
				groups.push({ type: "system", message: message });
				currentGroup = null;
				previousMessage = message;
				return;
			}

			if (!currentGroup || !shouldGroupWith(previousMessage, message)) {
				currentGroup = {
					type: "cluster",
					own: !!message.own,
					actor: message.actor || "Unknown",
					messages: []
				};
				groups.push(currentGroup);
			}

			currentGroup.messages.push(message);
			previousMessage = message;
		});

		return groups;
	}

	function renderTimeline(messages, emptyCopy, freshTailCount) {
		const indexedMessages = (messages || []).map(function(message, index) {
			return Object.assign({ renderIndex: index }, message);
		});
		const groups = renderMessageGroups(indexedMessages);
		refs.messageList.innerHTML = "";

		if (!groups.length) {
			const empty = document.createElement("div");
			empty.className = "empty-state";
			empty.innerHTML = "<h2>No history yet</h2><p></p>";
			empty.querySelector("p").textContent =
				emptyCopy || "Messages will appear here once the selected room has activity.";
			refs.messageList.appendChild(empty);
			return;
		}

		const freshStartIndex = Math.max(0, indexedMessages.length - Math.max(0, freshTailCount || 0));

		groups.forEach(function(group) {
			if (group.type === "day") {
				const divider = document.createElement("div");
				divider.className = "day-divider";
				divider.innerHTML = "<span></span><strong></strong><span></span>";
				divider.querySelector("strong").textContent = group.label;
				refs.messageList.appendChild(divider);
				return;
			}

			if (group.type === "system") {
				refs.messageList.appendChild(renderSystemMessage(group.message));
				return;
			}

			const firstMessage = group.messages[0];
			const cluster = document.createElement("section");
			cluster.className = "message-cluster" + (group.own ? " is-own" : "");
			cluster.style.setProperty("--avatar-hue", String(hueForLabel(group.actor, group.own)));
			cluster.classList.toggle("is-fresh", !!freshTailCount && group.messages.some(function(message) {
				return message.renderIndex >= freshStartIndex;
			}));

			if (!group.own) {
				const avatar = document.createElement("div");
				avatar.className = "message-avatar";
				styleAvatar(avatar, group.actor, false, firstMessage.avatarUrl || "");
				cluster.appendChild(avatar);
			}

			const stack = document.createElement("div");
			stack.className = "message-stack";

			const meta = document.createElement("div");
			meta.className = "message-meta";
			meta.innerHTML =
				"<span class=\"message-author\"></span><span class=\"message-time\"></span>";
			meta.querySelector(".message-author").textContent = group.own ? "" : group.actor;
			meta.querySelector(".message-time").textContent =
				group.own
					? group.messages[group.messages.length - 1].timeLabel || ""
					: firstMessage.timeLabel || "";
			stack.appendChild(meta);

			group.messages.forEach(function(message) {
				stack.appendChild(renderMessageBubble(message));
			});

			cluster.appendChild(stack);
			refs.messageList.appendChild(cluster);
		});
	}

	function messageListMetrics() {
		const maxScrollTop = Math.max(0, refs.messageList.scrollHeight - refs.messageList.clientHeight);
		const scrollTop = Math.max(0, refs.messageList.scrollTop);
		const distanceFromBottom = Math.max(0, maxScrollTop - scrollTop);
		return {
			scrollTop: scrollTop,
			maxScrollTop: maxScrollTop,
			distanceFromBottom: distanceFromBottom,
			nearBottom: distanceFromBottom <= 28
		};
	}

	function syncJumpLatestButton(metrics) {
		const state = metrics || messageListMetrics();
		const shouldShow = state.maxScrollTop > 0 && !state.nearBottom;
		refs.jumpLatestButton.classList.toggle("hidden", !shouldShow);
		refs.jumpLatestButton.textContent = unreadDetachedMessages > 0
			? "Jump to latest (" + String(unreadDetachedMessages) + " new)"
			: "Jump to latest";
	}

	function syncScrollState() {
		const metrics = messageListMetrics();
		if (metrics.nearBottom) {
			unreadDetachedMessages = 0;
		}

		keepMessageListPinnedToBottom = metrics.nearBottom || metrics.maxScrollTop <= 0;

		refs.appShell.classList.toggle("chat-has-overflow", metrics.maxScrollTop > 0);
		refs.appShell.classList.toggle("chat-is-scrolled", metrics.scrollTop > 8);
		refs.appShell.classList.toggle("chat-is-detached", !metrics.nearBottom);
		syncJumpLatestButton(metrics);
		return metrics;
	}

	function scheduleMessageListBottomPin(frameCount) {
		pendingBottomPinFrames = Math.max(pendingBottomPinFrames, Math.max(1, Number(frameCount) || 1));
		if (pendingBottomPinHandle) {
			return;
		}

		const applyBottomPin = function() {
			pendingBottomPinHandle = 0;
			if (pendingBottomPinFrames <= 0) {
				return;
			}

			scrollMessageListToBottom("auto");
			pendingBottomPinFrames -= 1;
			if (pendingBottomPinFrames > 0 && keepMessageListPinnedToBottom) {
				pendingBottomPinHandle = requestAnimationFrame(applyBottomPin);
			}
		};

		pendingBottomPinHandle = requestAnimationFrame(applyBottomPin);
		window.setTimeout(function() {
			if (keepMessageListPinnedToBottom) {
				scrollMessageListToBottom("auto");
			}
		}, 120);
	}

	function refreshMessageListPinning(frameCount) {
		if (keepMessageListPinnedToBottom) {
			scheduleMessageListBottomPin(frameCount || 2);
			return;
		}

		requestAnimationFrame(syncScrollState);
	}

	function ensureMessageListObservers() {
		if (messageListMutationObserver || typeof MutationObserver !== "function") {
			return;
		}

		messageListMutationObserver = new MutationObserver(function() {
			refreshMessageListPinning(2);
		});
		messageListMutationObserver.observe(refs.messageList, {
			childList: true,
			subtree: true,
			characterData: true
		});
	}

	function scrollMessageListToBottom(behavior) {
		unreadDetachedMessages = 0;
		const targetTop = Math.max(0, refs.messageList.scrollHeight - refs.messageList.clientHeight);
		refs.messageList.scrollTop = targetTop;
		if (typeof refs.messageList.scrollTo === "function") {
			try {
				refs.messageList.scrollTo({
					top: targetTop,
					behavior: behavior || "smooth"
				});
			} catch (error) {
				refs.messageList.scrollTop = targetTop;
			}
		}
		requestAnimationFrame(syncScrollState);
	}

	function monotonicNow() {
		return window.performance && typeof window.performance.now === "function"
			? window.performance.now()
			: Date.now();
	}

	function pauseReactionPickerScrollClose() {
		reactionPickerScrollClosePausedUntil = monotonicNow() + reactionPickerScrollCloseGraceMs;
	}

	function shouldKeepReactionPickerOpenOnScroll() {
		return openReactionPickerMessageId !== null && monotonicNow() <= reactionPickerScrollClosePausedUntil;
	}

	function renderMessages(snapshot) {
		const scope = snapshot.activeScope || {};
		const messages = snapshot.messages || [];
		const scopeToken = scope.scopeToken || [
			String(scope.kindLabel || ""),
			String(scope.label || ""),
			String(scope.description || "")
		].join("|");
		const scopeChanged = scopeToken !== lastScopeToken;
		const metricsBefore = messageListMetrics();
		const detachedBeforeRender = !scopeChanged && !metricsBefore.nearBottom;
		const distanceFromBottom = metricsBefore.distanceFromBottom;
		const latestTailKey = latestTailMessageKey(messages);
		const previousTailKey = scopeChanged ? "" : lastRenderedTailKey;
		const freshTailCount = latestTailKey && latestTailKey !== previousTailKey
			? countFreshTailMessages(messages, previousTailKey)
			: 0;

		if (scopeChanged) {
			unreadDetachedMessages = 0;
			openReactionPickerMessageId = null;
		}
		if (openReactionPickerMessageId !== null && !messages.some(function(message) {
			return message && message.messageId === openReactionPickerMessageId;
		})) {
			openReactionPickerMessageId = null;
		}

		if (detachedBeforeRender && freshTailCount > 0) {
			unreadDetachedMessages += freshTailCount;
		}

		if (scope.serverLogHtml) {
			refs.messageList.innerHTML = "";
			const log = document.createElement("div");
			log.className = "message-log";
			log.innerHTML = scope.serverLogHtml;
			refs.messageList.appendChild(log);
			requestAnimationFrame(function() {
				if (!detachedBeforeRender) {
					keepMessageListPinnedToBottom = true;
					scheduleMessageListBottomPin(3);
					return;
				}

				if (detachedBeforeRender) {
					refs.messageList.scrollTop = Math.max(0,
						refs.messageList.scrollHeight - refs.messageList.clientHeight - distanceFromBottom);
				}
				syncScrollState();
			});
			lastRenderedMessageCount = 0;
			lastRenderedTailKey = "";
			lastScopeToken = scopeToken;
			return;
		}

		renderTimeline(messages, scope.emptyCopy || "", freshTailCount);

		const shouldStickToBottom = (scope.scrollToBottom !== false)
			&& (scopeChanged || (!detachedBeforeRender && messages.length >= lastRenderedMessageCount));

		requestAnimationFrame(function() {
			if (shouldStickToBottom) {
				keepMessageListPinnedToBottom = true;
				scheduleMessageListBottomPin(4);
				return;
			}

			if (detachedBeforeRender) {
				refs.messageList.scrollTop = Math.max(0,
					refs.messageList.scrollHeight - refs.messageList.clientHeight - distanceFromBottom);
			}
			syncScrollState();
		});

		lastRenderedMessageCount = messages.length;
		lastRenderedTailKey = latestTailKey;
		lastScopeToken = scopeToken;
	}

	function renderNote(app, scope) {
		const motdHtml = String(app.motdHtml || "").trim();
		const motdSummary = String(app.motdSummary || "").trim();
		const hasMotd = !!motdHtml;
		refs.noteCard.classList.toggle("hidden", !hasMotd);
		if (!hasMotd) {
			noteExpanded = true;
			noteUserOverride = false;
			railNoteAutoCollapsed = false;
			lastMotdSignature = "";
			refs.noteCard.style.maxHeight = "";
			refs.serverSubtitle.style.maxHeight = "";
			refs.noteCard.classList.remove("is-expanded", "has-rich-note");
			refs.noteToggleButton.classList.add("hidden");
			refs.noteToggleButton.setAttribute("aria-expanded", "false");
			return;
		}

		if (lastMotdSignature !== motdHtml) {
			lastMotdSignature = motdHtml;
			noteExpanded = true;
			noteUserOverride = false;
			railNoteAutoCollapsed = false;
		}

		const fullText = plainTextFromHtml(motdHtml);
		const hasSummary = !!motdSummary && motdSummary !== fullText;
		const hasRichMotdLayout = /<(img|svg|table|h[1-6]|font|center|div|p|span)\b/i.test(motdHtml);
		const isExpandable = fullText.length > 0;
		const expanded = noteExpanded;

		refs.serverTitle.textContent = app.serverTitle || "Mumble";
		refs.noteCard.classList.toggle("is-expanded", expanded);
		refs.noteCard.classList.toggle("has-rich-note", hasRichMotdLayout);
		refs.noteToggleButton.classList.toggle("hidden", !isExpandable);
		refs.noteToggleButton.setAttribute("aria-expanded", expanded ? "true" : "false");
		refs.noteToggleButton.textContent = expanded ? "Hide" : "Show";
		refs.noteToggleButton.title = expanded ? "Hide message of the day" : "Show the full message of the day";

		if (expanded) {
			refs.serverSubtitle.innerHTML = "<div class=\"note-body-content\">" + motdHtml + "</div>";
			refs.serverSubtitle.classList.remove("is-collapsed");
			refs.serverSubtitle.title = "";
		} else {
			refs.serverSubtitle.innerHTML = hasSummary ? escapedMultilineText(motdSummary) : motdHtml;
			refs.serverSubtitle.classList.add("is-collapsed");
			refs.serverSubtitle.title = fullText;
		}

		scheduleRailLayoutSync();
	}

	function renderSelfCard(app) {
		refs.selfAvatar.className = "self-avatar";
		styleAvatar(refs.selfAvatar, app.selfName || "You", true, app.selfAvatarUrl || "");
		refs.selfName.textContent = app.selfName || "You";
		refs.selfStatus.textContent = app.selfStatusLabel || "Offline";
		refs.selfStatus.className = "self-status";
		if (app.selfStatusTone) {
			refs.selfStatus.classList.add("is-" + app.selfStatusTone);
		}
		if (app.selfStatusTone) {
			refs.selfAvatar.classList.add("is-" + app.selfStatusTone);
		}
		refs.selfCard.dataset.statusTone = app.selfStatusTone || "";
		refs.selfCard.setAttribute("aria-expanded", selfMenuOpen ? "true" : "false");
	}

	function fallbackMenus(app) {
		return [
			{
				id: "server",
				label: "Server",
				items: [
					{ id: "server.connect", label: "Connect", enabled: !!app.canConnect },
					{ id: "server.disconnect", label: "Disconnect", enabled: !!app.canDisconnect, tone: "danger" },
					{ id: "server.information", label: "Server info", enabled: !!app.canDisconnect },
					{ id: "server.favorite", label: "Add favorite", enabled: !!app.canDisconnect }
				]
			},
			{
				id: "self",
				label: "Self",
				items: [
					{ id: "self.comment", label: "Comment", enabled: !!app.canDisconnect },
					{ id: "self.register", label: "Register", enabled: !!app.canDisconnect },
					{ id: "self.prioritySpeaker", label: "Priority speaker", enabled: !!app.canDisconnect },
					{ id: "self.audioStats", label: "Audio stats", enabled: true }
				]
			},
			{
				id: "configure",
				label: "Configure",
				items: [
					{ id: "configure.settings", label: "Settings", enabled: true },
					{ id: "configure.screenShare", label: "Screen sharing settings", enabled: true },
					{ id: "configure.audioWizard", label: "Audio wizard", enabled: true },
					{ id: "configure.certificate", label: "Certificate wizard", enabled: true },
					{ id: "configure.minimal", label: "Minimal view", enabled: true },
					{ id: "configure.hideFrame", label: "Hide native window border", enabled: true }
				]
			},
			{
				id: "help",
				label: "Help",
				items: [
					{ id: "help.whatsThis", label: "What's this", enabled: true },
					{ id: "help.versionCheck", label: "Check for updates", enabled: true },
					{ id: "help.about", label: "About Mumble", enabled: true },
					{ id: "help.aboutQt", label: "About Qt", enabled: true }
				]
			}
		];
	}

	function resolvedAppMenus(app) {
		if (app && Array.isArray(app.menus) && app.menus.length) {
			return app.menus;
		}

		return fallbackMenus(app || {});
	}

	function hideAppMenu() {
		appMenuOpen = false;
		refs.appMenu.classList.add("hidden");
		refs.appMenu.setAttribute("aria-hidden", "true");
		refs.appMenu.innerHTML = "";
		refs.settingsButton.setAttribute("aria-expanded", "false");
	}

	function positionAppMenu() {
		if (refs.appMenu.classList.contains("hidden")) {
			return;
		}

		const anchorBounds = refs.settingsButton.getBoundingClientRect();
		const menuBounds = refs.appMenu.getBoundingClientRect();
		const left = Math.max(8, Math.min(anchorBounds.right - menuBounds.width, window.innerWidth - menuBounds.width - 8));
		const belowTop = anchorBounds.bottom + 10;
		const aboveTop = anchorBounds.top - menuBounds.height - 10;
		const top = (belowTop + menuBounds.height <= window.innerHeight - 8)
			? belowTop
			: Math.max(8, aboveTop);
		refs.appMenu.style.left = left + "px";
		refs.appMenu.style.top = top + "px";
	}

	function renderAppMenu(snapshot) {
		const app = snapshot.app || {};
		const menus = resolvedAppMenus(app);

		refs.appMenu.innerHTML = "";

		const header = document.createElement("div");
		header.className = "app-menu-header";

		const eyebrow = document.createElement("p");
		eyebrow.className = "app-menu-eyebrow";
		eyebrow.textContent = "Mumble menu";

		const title = document.createElement("p");
		title.className = "app-menu-title";
		title.textContent = app.serverTitle || "Mumble";

		const subtitle = document.createElement("p");
		subtitle.className = "app-menu-subtitle";
		subtitle.textContent = "Classic main menus collected behind the gear button.";

		header.appendChild(eyebrow);
		header.appendChild(title);
		header.appendChild(subtitle);
		refs.appMenu.appendChild(header);

		menus.forEach(function(menu) {
			if (!menu || !Array.isArray(menu.items) || !menu.items.length) {
				return;
			}

			const section = document.createElement("section");
			section.className = "app-menu-section";

			const heading = document.createElement("div");
			heading.className = "app-menu-section-heading";
			heading.textContent = menu.label || "Menu";

			const body = document.createElement("div");
			body.className = "app-menu-section-body";

			actionItemsFromActionStates(menu.items, {
				invokeAction: function(actionId) {
					hideAppMenu();
					notifyBridge("invokeAppAction", actionId);
				}
			}).forEach(function(item) {
				appendActionPanelItem(body, item, "menu", false);
			});

			section.appendChild(heading);
			section.appendChild(body);
			refs.appMenu.appendChild(section);
		});

		appMenuOpen = true;
		refs.appMenu.classList.remove("hidden");
		refs.appMenu.setAttribute("aria-hidden", "false");
		refs.settingsButton.setAttribute("aria-expanded", "true");
		positionAppMenu();
	}

	function toggleAppMenu(forceOpen) {
		const nextOpen = typeof forceOpen === "boolean" ? forceOpen : !appMenuOpen;
		if (!nextOpen) {
			hideAppMenu();
			return;
		}

		const snapshot = getSnapshot();
		hideContextMenu();
		hideSelfMenu();
		openMenuId = null;
		openMenuPinned = false;
		renderMenus(resolvedAppMenus(snapshot.app || {}));
		renderAppMenu(snapshot);
	}

	function hideSelfMenu() {
		selfMenuOpen = false;
		refs.selfMenu.classList.add("hidden");
		refs.selfMenu.setAttribute("aria-hidden", "true");
		refs.selfMenu.innerHTML = "";
		refs.selfCard.setAttribute("aria-expanded", "false");
	}

	function positionSelfMenu() {
		if (refs.selfMenu.classList.contains("hidden")) {
			return;
		}

		const anchorBounds = refs.selfCard.getBoundingClientRect();
		const menuBounds = refs.selfMenu.getBoundingClientRect();
		const left = Math.max(8, Math.min(anchorBounds.right - menuBounds.width + 12, window.innerWidth - menuBounds.width - 8));
		const top = Math.max(8, anchorBounds.top - menuBounds.height - 12);
		refs.selfMenu.style.left = left + "px";
		refs.selfMenu.style.top = top + "px";
	}

	function renderSelfMenu(snapshot) {
		const app = snapshot.app || {};
		const menuState = app.selfMenu || {};

		refs.selfMenu.innerHTML = "";

		const header = document.createElement("div");
		header.className = "self-menu-header";

		const avatar = document.createElement("div");
		avatar.className = "self-menu-avatar" + (menuState.statusTone ? " is-" + menuState.statusTone : "");
		styleAvatar(avatar, menuState.name || app.selfName || "You", true, app.selfAvatarUrl || "");

		const copy = document.createElement("div");
		copy.className = "self-menu-copy";

		const title = document.createElement("p");
		title.className = "self-menu-title";
		title.textContent = menuState.name || app.selfName || "You";

		const status = document.createElement("p");
		status.className = "self-menu-status" + (menuState.statusTone ? " is-" + menuState.statusTone : "");
		status.textContent = menuState.statusLabel || app.selfStatusLabel || "Offline";

		copy.appendChild(title);
		copy.appendChild(status);
		header.appendChild(avatar);
		header.appendChild(copy);
		refs.selfMenu.appendChild(header);

		(menuState.presence || []).forEach(function(item) {
			const button = document.createElement("button");
			button.type = "button";
			button.className = "self-menu-row is-presence"
				+ (item.checked ? " is-selected" : "")
				+ (item.tone ? " is-" + item.tone : "");
			button.disabled = item.enabled === false;
			if (item.hint) {
				button.title = item.hint;
			}

			const dot = document.createElement("span");
			dot.className = "self-menu-dot";

			const label = document.createElement("span");
			label.className = "self-menu-label";
			label.textContent = item.label || "Status";

			button.appendChild(dot);
			button.appendChild(label);
			button.addEventListener("click", function() {
				hideSelfMenu();
				if (item.enabled === false) {
					return;
				}
				notifyBridge("invokeAppAction", item.id || "");
			});
			refs.selfMenu.appendChild(button);
		});

		(menuState.actions || []).forEach(function(item) {
			if (!item || item.kind === "separator") {
				const separator = document.createElement("div");
				separator.className = "self-menu-separator";
				refs.selfMenu.appendChild(separator);
				return;
			}
			if (!item.id) {
				return;
			}

			const button = document.createElement("button");
			button.type = "button";
			button.className = "self-menu-row"
				+ (item.checked ? " is-selected" : "")
				+ (item.tone ? " is-" + item.tone : "");
			button.disabled = item.enabled === false;
			if (item.hint) {
				button.title = item.hint;
			}

			const label = document.createElement("span");
			label.className = "self-menu-label";
			label.textContent = item.label || "Action";

			button.appendChild(label);
			button.addEventListener("click", function() {
				hideSelfMenu();
				if (item.enabled === false) {
					return;
				}
				notifyBridge("invokeAppAction", item.id || "");
			});
			refs.selfMenu.appendChild(button);
		});

		selfMenuOpen = true;
		refs.selfMenu.classList.remove("hidden");
		refs.selfMenu.setAttribute("aria-hidden", "false");
		refs.selfCard.setAttribute("aria-expanded", "true");
		positionSelfMenu();
	}

	function toggleSelfMenu(forceOpen) {
		const nextOpen = typeof forceOpen === "boolean" ? forceOpen : !selfMenuOpen;
		if (!nextOpen) {
			hideSelfMenu();
			return;
		}

		hideAppMenu();
		hideContextMenu();
		renderSelfMenu(getSnapshot());
	}

	function syncComposerHeight() {
		refs.composerInput.style.height = "0px";
		refs.composerInput.style.height = Math.min(refs.composerInput.scrollHeight, 160) + "px";
		scheduleFooterAlignmentSync();
	}

	function syncFooterAlignment() {
		footerAlignmentFrame = 0;
		if (!refs.appShell || !refs.composerForm || !refs.selfCard) {
			return;
		}

		const composerHeight = refs.composerForm.getBoundingClientRect().height;
		if (!composerHeight) {
			refs.appShell.style.removeProperty("--footer-row-height");
			return;
		}

		const dpr = window.devicePixelRatio || 1;
		const selfCardStyle = window.getComputedStyle(refs.selfCard);
		const selfCardChromeHeight =
			Number.parseFloat(selfCardStyle.paddingTop || "0")
			+ Number.parseFloat(selfCardStyle.paddingBottom || "0")
			+ Number.parseFloat(selfCardStyle.borderTopWidth || "0")
			+ Number.parseFloat(selfCardStyle.borderBottomWidth || "0");
		const selfContentHeight = Math.max(
			refs.selfAvatar ? refs.selfAvatar.getBoundingClientRect().height : 0,
			refs.selfCopy ? refs.selfCopy.getBoundingClientRect().height : 0,
			refs.selfCardSettingsButton ? refs.selfCardSettingsButton.getBoundingClientRect().height : 0
		);
		const selfMinimumHeight = selfContentHeight + selfCardChromeHeight;
		const snappedHeight = Math.ceil(Math.max(composerHeight, selfMinimumHeight) * dpr) / dpr;
		refs.appShell.style.setProperty("--footer-row-height", snappedHeight + "px");
	}

	function scheduleFooterAlignmentSync() {
		if (footerAlignmentFrame) {
			cancelAnimationFrame(footerAlignmentFrame);
		}
		footerAlignmentFrame = requestAnimationFrame(syncFooterAlignment);
	}

	function ensureFooterAlignmentObservers() {
		if (footerAlignmentResizeObserver || typeof ResizeObserver !== "function") {
			return;
		}

		footerAlignmentResizeObserver = new ResizeObserver(scheduleFooterAlignmentSync);
		[refs.composerForm, refs.composerShell, refs.composerReplyBar, refs.selfAvatar, refs.selfCopy].forEach(function(element) {
			if (element) {
				footerAlignmentResizeObserver.observe(element);
			}
		});
	}

	function renderComposerReplyState(scope) {
		const hasReply = !!(scope && scope.hasPendingReply);
		refs.composerReplyBar.classList.toggle("hidden", !hasReply);
		refs.composerShell.classList.toggle("has-reply", hasReply);
		if (!hasReply) {
			refs.composerReplyLabel.textContent = "Replying";
			refs.composerReplySnippet.textContent = "";
			return;
		}

		refs.composerReplyLabel.textContent = scope.replyActor
			? "Replying to " + String(scope.replyActor)
			: "Replying";
		refs.composerReplySnippet.textContent = scope.replySnippet || "";
	}

	function syncAmbientState(snapshot) {
		const app = snapshot.app || {};
		const scope = snapshot.activeScope || {};
		const toneSource = scope.label || app.serverTitle || "Mumble";
		const scopeHue = hueForLabel(toneSource, false);
		refs.appShell.style.setProperty("--scope-hue", String(scopeHue));
		refs.appShell.dataset.scopeKind = String(scope.kindLabel || "conversation").toLowerCase().replace(/\s+/g, "-");
	}

	function syncMenuBandChrome() {
		if (!refs.modernHeader) {
			return;
		}

		const hasMenus = !!(refs.menuBar && refs.menuBar.children.length);
		refs.modernHeader.classList.toggle("has-app-menus", hasMenus);
		refs.modernHeader.classList.toggle("menu-is-open", openMenuId !== null);
	}

	function renderCurrentMenus() {
		renderMenus(resolvedAppMenus((getSnapshot().app || {})));
	}

	function cancelMenuDismissTimer() {
		if (!menuDismissTimer) {
			return;
		}

		window.clearTimeout(menuDismissTimer);
		menuDismissTimer = 0;
	}

	function closeTopMenu() {
		cancelMenuDismissTimer();
		openMenuPinned = false;
		if (openMenuId === null) {
			syncMenuBandChrome();
			return;
		}

		openMenuId = null;
		renderCurrentMenus();
	}

	function scheduleTransientMenuDismiss() {
		if (openMenuPinned || openMenuId === null || menuDismissTimer) {
			return;
		}

		menuDismissTimer = window.setTimeout(function() {
			menuDismissTimer = 0;
			if (!openMenuPinned && openMenuId !== null) {
				closeTopMenu();
			}
		}, 90);
	}

	function menuPeekHotZoneContains(clientX, clientY) {
		if (!refs.modernHeader || !refs.menuBar || !refs.menuBar.children.length) {
			return false;
		}

		const headerRect = refs.modernHeader.getBoundingClientRect();
		const menuRect = refs.menuBar.getBoundingClientRect();
		const menuWidth = Math.max(refs.menuBar.scrollWidth || 0, menuRect.width || 0);
		if (!menuWidth) {
			return false;
		}

		const left = Math.max(headerRect.left, menuRect.left - 12);
		const right = Math.min(headerRect.right, menuRect.left + menuWidth + 18);
		const top = Math.max(headerRect.top, menuRect.top - 10);
		const bottom = menuRect.top + Math.max(30, menuRect.height || 0) + 8;
		return clientX >= left && clientX <= right && clientY >= top && clientY <= bottom;
	}

	function syncMenuPeekState(clientX, clientY) {
		if (!refs.modernHeader || !refs.menuBar
			|| !Number.isFinite(clientX) || !Number.isFinite(clientY)) {
			return;
		}

		const target = document.elementFromPoint(clientX, clientY);
		const targetInMenu = !!(target && typeof target.closest === "function"
			&& target.closest(".menu-band, .menu-group, .menu-panel"));
		const peeking = targetInMenu || menuPeekHotZoneContains(clientX, clientY);
		refs.modernHeader.classList.toggle("menu-is-peeked", peeking);
		if (peeking) {
			cancelMenuDismissTimer();
			return;
		}

		scheduleTransientMenuDismiss();
	}

	function clearMenuPeekState() {
		if (refs.modernHeader) {
			refs.modernHeader.classList.remove("menu-is-peeked");
		}
		if (!openMenuPinned) {
			closeTopMenu();
		}
	}

	function renderMenus(menus) {
		const appMenus = (menus || []).filter(function(menu) {
			return !!menu;
		});

		if (openMenuId !== null && !appMenus.some(function(menu) {
			return menu.id === openMenuId;
		})) {
			openMenuId = null;
			openMenuPinned = false;
		}

		refs.menuBar.innerHTML = "";

		appMenus.forEach(function(menu) {
			const menuItems = menu.items || [];
			const group = document.createElement("div");
			group.className = "menu-group" + (openMenuId === menu.id ? " is-open" : "");

			const trigger = document.createElement("button");
			trigger.type = "button";
			trigger.className = "menu-trigger";
			trigger.textContent = menu.label || "Menu";
			trigger.setAttribute("aria-expanded", openMenuId === menu.id ? "true" : "false");
			if (menuItems.length) {
				trigger.setAttribute("aria-haspopup", "menu");
			}
			trigger.addEventListener("mouseenter", function() {
				if (!menuItems.length || openMenuId === menu.id) {
					return;
				}

				hideAppMenu();
				hideContextMenu();
				openMenuPinned = false;
				openMenuId = menu.id;
				renderMenus(appMenus);
			});
			trigger.addEventListener("keydown", function(event) {
				if (event.key !== "ArrowDown" && event.key !== "Enter" && event.key !== " ") {
					return;
				}

				if (!menuItems.length) {
					return;
				}

				event.preventDefault();
				hideAppMenu();
				hideContextMenu();
				openMenuPinned = true;
				openMenuId = menu.id;
				renderMenus(appMenus);
				const activeGroup = refs.menuBar.querySelector(".menu-group.is-open");
				const firstItem = activeGroup ? activeGroup.querySelector(".menu-item:not(:disabled)") : null;
				if (firstItem) {
					firstItem.focus();
				}
			});
			trigger.addEventListener("click", function(event) {
				event.stopPropagation();
				hideAppMenu();
				hideContextMenu();
				if (openMenuId === menu.id && openMenuPinned) {
					openMenuId = null;
					openMenuPinned = false;
				} else {
					openMenuId = menu.id;
					openMenuPinned = true;
				}
				renderMenus(appMenus);
			});
			group.appendChild(trigger);

			if (menuItems.length) {
				const panel = document.createElement("div");
				panel.className = "menu-panel";
				panel.setAttribute("role", "menu");

				actionItemsFromActionStates(menuItems, {
					invokeAction: function(actionId) {
						notifyBridge("invokeAppAction", actionId);
					}
				}).forEach(function(item) {
					appendActionPanelItem(panel, item, "menu", true);
				});

				group.appendChild(panel);
			}

			refs.menuBar.appendChild(group);
		});

		syncMenuBandChrome();
	}

	function render(snapshot) {
		const app = snapshot.app || {};
		const scope = snapshot.activeScope || {};
		const textRooms = snapshot.textRooms || [];
		const voiceRooms = snapshot.voiceRooms || [];
		const voicePresence = snapshot.voicePresence || [];
		const headerPresence = voicePresence.length ? voicePresence : (snapshot.participants || []);

		refs.brandTitle.textContent = app.serverTitle || "Mumble";
		refs.brandSubtitle.textContent = app.serverSubtitle || "Room-first shell";
		refs.serverEyebrow.textContent = app.serverEyebrow || scope.kindLabel || "Mumble";
		refs.scopeTitle.textContent = scope.label || "Modern Layout";
		refs.scopeDescription.textContent = scope.description || "Select a room to see shared history.";

		applyStatePill(refs.layoutPill, app.layoutLabel || "Modern", app.layoutTone || "");
		applyStatePill(refs.connectionPill, app.connectionLabel || "Disconnected", app.connectionTone || "");
		applyStatePill(refs.compatPill, app.compatibilityLabel || "Standard server", app.compatibilityTone || "");
		refs.layoutSwitchButton.disabled = app.canToggleLayout === false;
		refs.layoutSwitchButton.title = app.layoutSwitchLabel || "Switch layout";
		refs.layoutSwitchButton.setAttribute("aria-label", app.layoutSwitchLabel || "Switch layout");

		refs.connectButton.disabled = !app.canConnect;
		refs.disconnectButton.disabled = !app.canDisconnect;
		refs.muteButton.classList.toggle("is-active", !!app.selfMuted);
		refs.deafButton.classList.toggle("is-active", !!app.selfDeafened);
		renderMenus(resolvedAppMenus(app));
		refs.settingsButton.setAttribute("aria-expanded", appMenuOpen ? "true" : "false");

		refs.textRoomCount.textContent = String(textRooms.length);
		refs.voiceRoomCount.textContent = String(voiceRooms.length);
		renderRoomList(refs.voiceRoomList, voiceRooms, {
			joinable: true,
			voicePresence: voicePresence,
			rootLabel: app.voiceRootLabel || "",
			rootToken: app.voiceRootScopeToken || ""
		});
		renderRoomList(refs.textRoomList, textRooms, {
			joinable: false,
			voicePresence: null,
			hideWhenEmpty: true
		});
		const renderedActiveRailToken = activeRailToken();
		if (!renderedActiveRailToken) {
			lastActiveRailToken = "";
		} else if (renderedActiveRailToken !== lastActiveRailToken) {
			lastActiveRailToken = renderedActiveRailToken;
			pendingActiveRailReveal = true;
		}

		renderVoicePresenceStack(headerPresence);
		renderMeta(scope.meta || []);
		renderScreenShareHeader(scope, scope.screenShare || null);
		renderScreenShareCard(scope, scope.screenShare || null);
		renderNote(app, scope);
		renderMessages(snapshot);
		renderSelfCard(app);
		syncAmbientState(snapshot);
		if (appMenuOpen) {
			renderAppMenu(snapshot);
		}
		if (selfMenuOpen) {
			renderSelfMenu(snapshot);
		}

		refs.scopeBanner.textContent = scope.banner || "";
		refs.scopeBanner.classList.toggle("hidden", !scope.banner);
		refs.loadOlderButton.disabled = !scope.canLoadOlder;
		refs.markReadButton.disabled = !scope.canMarkRead;
		refs.composerInput.disabled = !scope.canSend;
		refs.attachButton.disabled = !scope.canAttachImages;
		refs.sendButton.disabled = !scope.canSend;
		refs.composerInput.placeholder = scope.composerPlaceholder || "Write a message";
		refs.composerHint.textContent = scope.composerHint || "Persistent room history stays with the selected room.";
		renderComposerReplyState(scope);
		applyRailPresentation();
		syncComposerHeight();
		scheduleRailLayoutSync();

		if (scope.autoMarkRead) {
			notifyBridge("markRead");
		}
	}

	function syncSnapshot() {
		scheduleSnapshotRender();
	}

	function hideContextMenu() {
		contextMenuState = null;
		refs.contextMenu.classList.add("hidden");
		refs.contextMenu.setAttribute("aria-hidden", "true");
		refs.contextMenu.innerHTML = "";
	}

	function copyPlainText(text) {
		const value = String(text || "");
		if (!value) {
			return Promise.resolve(false);
		}

		if (modernBridge && typeof modernBridge.setClipboardText === "function") {
			try {
				modernBridge.setClipboardText(value);
				return Promise.resolve(true);
			} catch (error) {
				console.warn("Unable to write clipboard text:", error);
			}
		}

		if (navigator.clipboard && typeof navigator.clipboard.writeText === "function") {
			return navigator.clipboard.writeText(value).then(function() {
				return true;
			}).catch(function() {
				return false;
			});
		}

		const scratch = document.createElement("textarea");
		scratch.value = value;
		scratch.setAttribute("readonly", "readonly");
		scratch.style.position = "fixed";
		scratch.style.left = "-9999px";
		document.body.appendChild(scratch);
		scratch.select();
		const copied = document.execCommand("copy");
		document.body.removeChild(scratch);
		return Promise.resolve(copied);
	}

	function replaceComposerSelection(replacement) {
		const input = refs.composerInput;
		const start = input.selectionStart || 0;
		const end = input.selectionEnd || 0;
		const before = input.value.slice(0, start);
		const after = input.value.slice(end);
		input.value = before + replacement + after;
		const caret = start + replacement.length;
		input.selectionStart = caret;
		input.selectionEnd = caret;
		syncComposerHeight();
		input.dispatchEvent(new Event("input", { bubbles: true }));
	}

	function buildRoomContextMenuItems(snapshot, room, roomRow) {
		const scope = (snapshot && snapshot.activeScope) || {};
		const roomToken = (room && room.token) || (roomRow && roomRow.dataset.scopeToken) || "";
		const isVoiceRoom = roomRow && roomRow.dataset.roomType === "voice";
		const items = [
			{
				label: "Open room",
				enabled: !!roomToken,
				action: function() {
					notifyBridge("selectScope", roomToken);
				}
			}
		];

		if (isVoiceRoom) {
			items.push({
				label: "Join voice",
				enabled: roomRow && roomRow.dataset.canJoin === "true",
				action: function() {
					notifyBridge("joinVoiceChannel", roomToken);
				}
			});
		}

		if (room && room.participantSession) {
			const participantItems = actionItemsFromActionStates(room.participantActions, {
				invokeAction: function(actionId) {
					notifyBridge("invokeParticipantAction", room.participantSession, actionId);
				},
				invokeValueChanged: function(actionId, value, final) {
					notifyBridge("participantActionValueChanged", room.participantSession, actionId, value, final);
				}
			});
			if (participantItems.length) {
				items.push({ separator: true });
				return items.concat(participantItems);
			}
		}

		const roomActionItems = room
			? actionItemsFromActionStates(room.actions, {
				invokeAction: function(actionId) {
					notifyBridge("invokeScopeAction", room.token || roomToken, actionId);
				},
				invokeValueChanged: function(actionId, value, final) {
					notifyBridge("scopeActionValueChanged", room.token || roomToken, actionId, value, final);
				}
			})
			: [];
		if (roomActionItems.length) {
			items.push({ separator: true });
			items.push.apply(items, roomActionItems);
		}

		if (scope.canMarkRead) {
			items.push({ separator: true });
			items.push({
				label: "Mark read",
				enabled: true,
				action: function() {
					notifyBridge("markRead");
				}
			});
		}

		return items;
	}

	function buildContextMenuItems(event) {
		const snapshot = getSnapshot();
		const scope = snapshot.activeScope || {};
		const roomRow = event.target.closest(".rail-row");
		const presenceActionButton = event.target.closest(".presence-action-button");
		const presenceRow = event.target.closest(".presence-row")
			|| (presenceActionButton && presenceActionButton.parentElement
				? presenceActionButton.parentElement.querySelector(".presence-row")
				: null);
		const bubble = event.target.closest(".message-bubble");
		const composer = event.target.closest("#composer-input");
		const selfCard = event.target.closest("#self-card");

		if (composer) {
			const input = refs.composerInput;
			const hasSelection = (input.selectionEnd || 0) > (input.selectionStart || 0);
			return [
				{
					label: "Cut",
					enabled: hasSelection,
					action: function() {
						copyPlainText(input.value.slice(input.selectionStart || 0, input.selectionEnd || 0)).then(function(copied) {
							if (copied) {
								replaceComposerSelection("");
							}
						});
					}
				},
				{
					label: "Copy",
					enabled: hasSelection,
					action: function() {
						copyPlainText(input.value.slice(input.selectionStart || 0, input.selectionEnd || 0));
					}
				},
				{
					label: "Paste",
					enabled: !input.disabled,
					action: function() {
						input.focus();
						if (!!scope.canAttachImages) {
							bridgeClipboardHasImage(function(hasImage) {
								if (hasImage) {
									notifyBridge("attachClipboardImage");
									return;
								}

								pastePlainTextIntoComposer();
							});
							return;
						}

						pastePlainTextIntoComposer();
					}
				},
				{
					label: "Attach image",
					enabled: !!scope.canAttachImages,
					action: function() {
						notifyBridge("openImagePicker");
					}
				},
				{
					label: "Clear",
					enabled: !!input.value,
					action: function() {
						input.value = "";
						syncComposerHeight();
						input.dispatchEvent(new Event("input", { bubbles: true }));
					}
				}
			];
		}

		if (presenceRow) {
			const participant = findParticipantState(snapshot, presenceRow.dataset.participantKey || presenceRow.dataset.session);
			return participantContextMenuItems(participant, presenceRow.dataset.scopeToken);
		}

		if (roomRow) {
			const room = findRoomState(snapshot, roomRow.dataset.scopeToken);
			return buildRoomContextMenuItems(snapshot, room, roomRow);
		}

		if (bubble) {
			const message = findMessageState(snapshot, bubble.dataset.messageId);
			const items = [
				{
					label: "Copy message",
					enabled: !!bubble.dataset.bodyText,
					action: function() {
						copyPlainText(bubble.dataset.bodyText);
					}
				}
			];
			if (message && message.canReply) {
				items.push({
					label: "Reply",
					enabled: true,
					action: function() {
						notifyBridge("startReply", message.messageId);
					}
				});
			}
			if (message && message.canReact) {
				items.push({
					label: "Add reaction",
					enabled: true,
					action: function() {
						openReactionPickerMessageId = message.messageId;
						pauseReactionPickerScrollClose();
						syncSnapshot();
					}
				});
			}
			if (message && message.canDelete) {
				items.push({
					label: "Delete message",
					enabled: true,
					tone: "danger",
					action: function() {
						requestDeleteMessage(message);
					}
				});
			}
			items.push(
				{
					separator: true
				},
				{
					label: "Load older",
					enabled: !!scope.canLoadOlder,
					action: function() {
						notifyBridge("loadOlderHistory");
					}
				},
				{
					label: "Mark read",
					enabled: !!scope.canMarkRead,
					action: function() {
						notifyBridge("markRead");
					}
				}
			);
			return items;
		}

		if (selfCard) {
			return [];
		}

		return [
			{
				label: "Load older",
				enabled: !!scope.canLoadOlder,
				action: function() {
					notifyBridge("loadOlderHistory");
				}
			},
			{
				label: "Mark read",
				enabled: !!scope.canMarkRead,
				action: function() {
					notifyBridge("markRead");
				}
			},
			{
				label: "Attach image",
				enabled: !!scope.canAttachImages,
				action: function() {
					notifyBridge("openImagePicker");
				}
			}
		];
	}

	function showContextMenu(items, clientX, clientY) {
		hideAppMenu();
		hideSelfMenu();
		const filteredItems = [];
		(items || []).forEach(function(item) {
			const kind = actionPanelItemKind(item);
			if (!kind) {
				return;
			}
			if (kind === "separator"
				&& (!filteredItems.length || actionPanelItemKind(filteredItems[filteredItems.length - 1]) === "separator")) {
				return;
			}
			filteredItems.push(item);
		});
		if (filteredItems.length && actionPanelItemKind(filteredItems[filteredItems.length - 1]) === "separator") {
			filteredItems.pop();
		}
		if (!filteredItems.length) {
			hideContextMenu();
			return;
		}

		refs.contextMenu.innerHTML = "";
		filteredItems.forEach(function(item) {
			appendActionPanelItem(refs.contextMenu, item, "context", true);
		});

		refs.contextMenu.classList.remove("hidden");
		refs.contextMenu.setAttribute("aria-hidden", "false");
		const bounds = refs.contextMenu.getBoundingClientRect();
		const left = Math.max(8, Math.min(clientX, window.innerWidth - bounds.width - 8));
		const top = Math.max(8, Math.min(clientY, window.innerHeight - bounds.height - 8));
		refs.contextMenu.style.left = left + "px";
		refs.contextMenu.style.top = top + "px";
		contextMenuState = { left: left, top: top };
	}

	function wireActions() {
		document.addEventListener("click", handleAnchorActivation, true);
		document.addEventListener("pointermove", function(event) {
			syncMenuPeekState(event.clientX, event.clientY);
		}, true);
		document.addEventListener("mouseout", function(event) {
			if (!event.relatedTarget) {
				clearMenuPeekState();
			}
		});
		refs.connectButton.addEventListener("click", function() { notifyBridge("openConnectDialog"); });
		refs.disconnectButton.addEventListener("click", function() { notifyBridge("disconnectServer"); });
		refs.layoutSwitchButton.addEventListener("click", function() { notifyBridge("toggleLayout"); });
		refs.settingsButton.addEventListener("click", function(event) {
			event.stopPropagation();
			toggleAppMenu();
		});
		refs.muteButton.addEventListener("click", function() { notifyBridge("toggleSelfMute"); });
		refs.deafButton.addEventListener("click", function() { notifyBridge("toggleSelfDeaf"); });
		refs.screenShareButton.addEventListener("click", function() {
			const scopeToken = refs.screenShareButton.dataset.scopeToken || "";
			const actionId = refs.screenShareButton.dataset.actionId || "";
			if (!scopeToken || !actionId || refs.screenShareButton.disabled) {
				return;
			}
			notifyBridge("invokeScopeAction", scopeToken, actionId);
		});
		refs.loadOlderButton.addEventListener("click", function() { notifyBridge("loadOlderHistory"); });
		refs.markReadButton.addEventListener("click", function() { notifyBridge("markRead"); });
		refs.attachButton.addEventListener("click", function() { notifyBridge("openImagePicker"); });
		refs.composerReplyCancelButton.addEventListener("click", function() {
			notifyBridge("cancelReply");
			refs.composerInput.focus();
		});
		refs.selfCard.addEventListener("click", function(event) {
			if (event.target.closest("#self-card-settings-button")) {
				return;
			}
			toggleSelfMenu();
		});
		refs.selfCard.addEventListener("keydown", function(event) {
			if (event.key === "Enter" || event.key === " ") {
				event.preventDefault();
				toggleSelfMenu();
			}
		});
		refs.selfCardSettingsButton.addEventListener("click", function(event) {
			event.stopPropagation();
			notifyBridge("openSettings");
		});
		refs.noteToggleButton.addEventListener("click", function() {
			noteUserOverride = true;
			railNoteAutoCollapsed = false;
			noteExpanded = !noteExpanded;
			renderNote(getSnapshot().app || {}, getSnapshot().activeScope || {});
		});
		refs.jumpLatestButton.addEventListener("click", function() {
			keepMessageListPinnedToBottom = true;
			scrollMessageListToBottom("smooth");
		});
		refs.imageViewerBackdrop.addEventListener("click", closeImageViewer);
		refs.imageViewerCloseButton.addEventListener("click", closeImageViewer);
		refs.imageViewerHeader.addEventListener("mousedown", beginImageViewerDrag);
		refs.imageViewerImage.addEventListener("dragstart", function(event) {
			event.preventDefault();
		});
		refs.railToggleButton.addEventListener("click", function() {
			setRailCollapsed(!railCollapsed);
		});
		refs.utilityRailBackdrop.addEventListener("click", function() {
			setRailCollapsed(true);
		});
		refs.railCloseButton.addEventListener("click", function() {
			setRailCollapsed(true);
		});
		refs.composerInput.addEventListener("input", syncComposerHeight);
		refs.composerInput.addEventListener("keydown", handleComposerInputKeyDown);
		refs.composerInput.addEventListener("paste", handleComposerImagePaste);
		refs.composerForm.addEventListener("submit", function(event) {
			event.preventDefault();
			sendComposerDraft();
		});
		[refs.composerForm, refs.messageList].forEach(function(target) {
			target.addEventListener("dragenter", handleComposerImageDragEnter);
			target.addEventListener("dragover", handleComposerImageDragOver);
			target.addEventListener("dragleave", clearComposerImageDropTarget);
			target.addEventListener("drop", handleComposerImageDrop);
		});
		document.addEventListener("click", function(event) {
			if (!event.target.closest(".context-menu")) {
				hideContextMenu();
			}
			if (!event.target.closest(".app-menu-popover") && !event.target.closest("#settings-button")) {
				hideAppMenu();
			}
			if (!event.target.closest(".self-menu-popover") && !event.target.closest("#self-card")) {
				hideSelfMenu();
			}
			if (!event.target.closest(".reaction-picker") && !event.target.closest(".reaction-picker-toggle")) {
				if (openReactionPickerMessageId !== null) {
					openReactionPickerMessageId = null;
					syncSnapshot();
				}
			}
			if (!event.target.closest(".menu-group") && openMenuId !== null) {
				closeTopMenu();
			}
		});
		document.addEventListener("contextmenu", function(event) {
			const contextTarget = eventElementTarget(event);
			if (contextTarget && typeof contextTarget.closest === "function" && contextTarget.closest(".image-viewer-layer")) {
				event.preventDefault();
				return;
			}
			if (event.target.closest(".context-menu")) {
				return;
			}
			if (event.target.closest("#self-card")) {
				event.preventDefault();
				toggleSelfMenu(true);
				return;
			}
			const items = buildContextMenuItems(event).filter(function(item) {
				return !!item;
			});
			if (!items.length) {
				hideContextMenu();
				return;
			}
			event.preventDefault();
			showContextMenu(items, event.clientX, event.clientY);
		});
		window.addEventListener("keydown", function(event) {
			if (event.key === "Escape" && refs.imageViewerLayer && !refs.imageViewerLayer.classList.contains("hidden")) {
				closeImageViewer();
				return;
			}
			if (event.key === "Escape" && compactViewport && !railCollapsed) {
				setRailCollapsed(true);
				return;
			}
			if (event.key === "Escape" && appMenuOpen) {
				hideAppMenu();
				return;
			}
			if (event.key === "Escape" && openMenuId !== null) {
				closeTopMenu();
				return;
			}
			if (event.key === "Escape" && openReactionPickerMessageId !== null) {
				openReactionPickerMessageId = null;
				syncSnapshot();
				return;
			}
			if (event.key === "Escape") {
				hideContextMenu();
				hideSelfMenu();
			}
		});
		window.addEventListener("resize", function() {
			syncCompactRailState(false);
			hideContextMenu();
			if (appMenuOpen) {
				positionAppMenu();
			}
			if (selfMenuOpen) {
				positionSelfMenu();
			}
			if (refs.imageViewerLayer && !refs.imageViewerLayer.classList.contains("hidden")) {
				applyImageViewerGeometry();
				persistImageViewerState();
			}
			refreshMessageListPinning(2);
			scheduleFooterAlignmentSync();
			scheduleRailLayoutSync();
		});
		if (window.visualViewport) {
			window.visualViewport.addEventListener("resize", scheduleFooterAlignmentSync);
		}
		window.addEventListener("blur", function() {
			clearMenuPeekState();
			hideContextMenu();
			hideAppMenu();
			hideSelfMenu();
			clearComposerImageDropTarget();
		});
		refs.messageList.addEventListener("load", function(event) {
			if (!event.target || event.target.tagName !== "IMG") {
				return;
			}
			refreshMessageListPinning(2);
		}, true);
		refs.utilityRail.addEventListener("load", function(event) {
			if (!event.target || event.target.tagName !== "IMG") {
				return;
			}
			scheduleRailLayoutSync();
		}, true);
		refs.utilityScroll.addEventListener("wheel", handleUtilityWheel, { passive: false });
		refs.utilityScroll.addEventListener("scroll", function() {
			hideContextMenu();
			hideSelfMenu();
			syncRailOverflowState();
		});
		refs.messageList.addEventListener("click", handleMessageImageActivation);
		refs.messageList.addEventListener("scroll", function() {
			if (contextMenuState) {
				hideContextMenu();
			}
			if (selfMenuOpen) {
				hideSelfMenu();
			}
			const shouldCloseReactionPicker =
				openReactionPickerMessageId !== null && !shouldKeepReactionPickerOpenOnScroll();
			if (shouldCloseReactionPicker) {
				openReactionPickerMessageId = null;
				reactionPickerScrollClosePausedUntil = 0;
			}
			syncScrollState();
			if (shouldCloseReactionPicker) {
				syncSnapshot();
			}
		});
	}

	async function boot() {
		wireActions();
		ensureMessageListObservers();
		ensureFooterAlignmentObservers();
		syncCompactRailState(true);
		syncComposerHeight();
		await ensureBridge();
		syncSnapshot();
	}

	boot();
})();
