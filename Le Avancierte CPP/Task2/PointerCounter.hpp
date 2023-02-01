#pragma once
class PointerCounter {
private:
	size_t hardReferences;
	size_t weakReferences;
public:
	PointerCounter() noexcept : hardReferences(0), weakReferences(0) {};
	
	void addHardReference() noexcept { ++hardReferences; }
	void removeHardReference() noexcept { --hardReferences; }
	void addWeakReference() noexcept { ++weakReferences; }
	void removeWeakReference() noexcept { --weakReferences; }

	size_t hardCount() const noexcept { return hardReferences; }
	size_t weakCount() const noexcept { return weakReferences; }

	bool CanDestroy() const noexcept { return hardReferences <= 0; }
	bool Empty() const noexcept { return hardReferences <= 0 && weakReferences <= 0; }
};

//PointerCounter::PointerCounter() noexcept : hardReferences(0), weakReferences(0) {};

//void PointerCounter::addHardReference() noexcept { ++hardReferences; }
//void PointerCounter::removeHardReference() noexcept {--hardReferences; }
//void PointerCounter::addWeakReference() noexcept { ++weakReferences; }
//void PointerCounter::removeWeakReference() noexcept {--weakReferences; }

//size_t PointerCounter::hardCount() const noexcept { return hardReferences; }
//size_t PointerCounter::weakCount() const noexcept { return weakReferences; }
//
//bool PointerCounter::CanDestroy() const noexcept {return hardReferences <= 0;}
//bool PointerCounter::Empty() const noexcept { return hardReferences <= 0 && weakReferences <= 0; }
