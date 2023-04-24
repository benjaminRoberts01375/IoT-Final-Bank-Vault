// Ben Roberts

import SwiftUI

struct ContentView: View {
    @StateObject var controller: ContentViewModel = ContentViewModel()
    
    var body: some View {
        NavigationStack {
            List {
                ForEach(Array(UserLoginM.shared.vaults.enumerated()), id: \.element.id) { index, vault in
                    Button(action: {
                        
                    }, label: {
                        Text(vault.name)
                    })
                    .transition(.opacity)
                }
            }
            .edgesIgnoringSafeArea([.bottom, .horizontal])
            .navigationTitle("Your Vaults")
            .toolbar {
                if controller.editMode {
                    Button(action: {                                // Add vault button
                        print("Add a vault")
                    }, label: {
                        Text("Add")
                    })
                    Button(action: {                                // Finish editing button
                        withAnimation {
                            controller.editMode.toggle()
                        }
                    }, label: {
                        Text("Done")
                    })
                    .disabled(UserLoginM.shared.vaults.isEmpty)
                }
                else {
                    Button(action: {                                // Edit mode button
                        controller.editMode.toggle()
                    }, label: {
                        Text("Edit")
                    })
                }
            }
            .fullScreenCover(isPresented: $controller.showLoginPage) {
                UserLoginV()
            }
        }
    }
}

struct ContentView_Previews: PreviewProvider {
    static var previews: some View {
        ContentView()
    }
}
