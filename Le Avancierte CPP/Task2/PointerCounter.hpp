#pragma once
class PointerCounter {
private:
	size_t hardReferences;
	size_t weakReferences;
public:
	PointerCounter() noexcept;
	
	void AddHardReference() noexcept;
	void RemoveHardReference() noexcept;
	void AddWeakReference() noexcept;
	void RemoveWeakReference() noexcept;

	size_t hardCount() const noexcept;
	size_t weakCount() const noexcept;

	bool CanDestroy() const noexcept;
	bool Empty() const noexcept;
};

PointerCounter::PointerCounter() noexcept : hardReferences(0), weakReferences(0) {};

void PointerCounter::AddHardReference() noexcept { ++hardReferences; }
void PointerCounter::RemoveHardReference() noexcept {--hardReferences; }
void PointerCounter::AddWeakReference() noexcept { ++weakReferences; }
void PointerCounter::RemoveWeakReference() noexcept {--weakReferences; }

size_t PointerCounter::hardCount() const noexcept { return hardReferences; }
size_t PointerCounter::weakCount() const noexcept { return weakReferences; }

bool PointerCounter::CanDestroy() const noexcept {return hardReferences <= 0;}
bool PointerCounter::Empty() const noexcept { return hardReferences <= 0 && weakReferences <= 0; }
