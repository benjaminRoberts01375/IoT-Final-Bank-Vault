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
                    Button(action: {
                        print("Add a vault")
                    }, label: {
                        Text("Add")
                    })
                    Button(action: {
                        withAnimation {
                            controller.editMode.toggle()
                        }
                    }, label: {
                        Text("Done")
                    })
                    .disabled(UserLoginM.shared.vaults.isEmpty)
                }
                else {
                    Button(action: {
                        print("Toggle edit mode")
                    }, label: {
                        Text("Edit")
                    })
                }
            }
        }
    }
}

struct ContentView_Previews: PreviewProvider {
    static var previews: some View {
        ContentView()
    }
}
